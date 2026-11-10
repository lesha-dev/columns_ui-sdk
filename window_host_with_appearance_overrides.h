#pragma once

namespace uie {

/**
 * \brief Subclass of window_host that lets the host replace the fonts and colours used by the windows it hosts.
 *
 * New in SDK version 8.2.0.
 *
 * Implement this interface in your window host if the panels you host should use fonts and colours chosen by
 * your component instead of the ones configured in Columns UI Colours and fonts preferences. For example,
 * a skinned host may want a playlist view to use the font and colours of its skin.
 *
 * Hosted windows retrieve the interface by calling `service_query_t()` on the window host they were
 * created with. Windows that don’t support overrides, and versions of Columns UI that predate this
 * interface, ignore it and use the configured fonts and colours.
 *
 * \note In Columns UI, font and colour overrides are supported by the playlist view, Filter, Item properties
 *       and Playlist switcher panels.
 *
 * \par Usage example (hosted window)
 * \code{.cpp}
 * cui::fonts::font::ptr get_font(const uie::window_host::ptr& host, GUID font_id)
 * {
 *     uie::window_host_with_appearance_overrides::ptr overrides;
 *
 *     if (host.is_valid() && host->service_query_t(overrides)) {
 *         if (auto font = overrides->get_font_override(font_id); font.is_valid())
 *             return font;
 *     }
 *
 *     return fb2k::std_api_get<cui::fonts::manager_v3>()->get_font(font_id);
 * }
 *
 * COLORREF get_colour(const uie::window_host::ptr& host, GUID colour_client_id,
 *     cui::colours::colour_identifier_t colour_id)
 * {
 *     uie::window_host_with_appearance_overrides::ptr overrides;
 *     COLORREF colour{};
 *
 *     if (host.is_valid() && host->service_query_t(overrides)
 *         && overrides->get_colour_override(colour_client_id, colour_id, colour))
 *         return colour;
 *
 *     return cui::colours::helper(colour_client_id).get_colour(colour_id);
 * }
 * \endcode
 */
class NOVTABLE window_host_with_appearance_overrides : public window_host {
public:
    /**
     * \brief Get the font to use instead of the configured font for a font client.
     *
     * \pre May only be called by a hosted window.
     *
     * \param[in] font_id   GUID of the font client (see cui::fonts::client). Use
     *                      cui::fonts::client::create_by_guid() to find out its default font type.
     * \return font to use, or an empty pointer to use the font configured in Columns UI
     */
    [[nodiscard]] virtual cui::fonts::font::ptr get_font_override(GUID font_id) = 0;

    /**
     * \brief Register a callback for when the fonts returned by get_font_override() have changed.
     *
     * Hosted windows should call get_font_override() again for each of their fonts when the
     * callback is called.
     *
     * \pre May only be called by a hosted window.
     *
     * \param[in] callback   callback function
     * \return callback token that should be reset when you no longer want the callback function to be called
     */
    [[nodiscard]] virtual cui::callback_token::ptr on_font_overrides_changed(cui::basic_callback::ptr callback) = 0;

    /**
     * \brief Get the colour to use instead of the configured colour for a colour client.
     *
     * When a host overrides the selection background colour, hosted windows should draw selected
     * items using the overridden colours rather than using themed selection rendering.
     *
     * \pre May only be called by a hosted window.
     *
     * \param[in] colour_client_id   GUID of the colour client (see cui::colours::client)
     * \param[in] colour_id          colour to get
     * \param[out] colour            receives the colour to use, if overridden
     * \return true if the colour is overridden, false to use the colour configured in Columns UI
     */
    [[nodiscard]] virtual bool get_colour_override(
        GUID colour_client_id, cui::colours::colour_identifier_t colour_id, COLORREF& colour) = 0;

    /**
     * \brief Register a callback for when the colours returned by get_colour_override() have changed.
     *
     * Hosted windows should call get_colour_override() again for each of their colours, and redraw, when
     * the callback is called.
     *
     * \pre May only be called by a hosted window.
     *
     * \param[in] callback   callback function
     * \return callback token that should be reset when you no longer want the callback function to be called
     */
    [[nodiscard]] virtual cui::callback_token::ptr on_colour_overrides_changed(cui::basic_callback::ptr callback) = 0;

    FB2K_MAKE_SERVICE_INTERFACE(window_host_with_appearance_overrides, window_host);
};

} // namespace uie
