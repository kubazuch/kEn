#pragma once

#include <mEn/vec2.hpp>

#include <kEn/core/input/key_codes.hpp>
#include <kEn/core/input/mouse_codes.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Stateless input API for querying keyboard and mouse state.
 *
 * All functions require the windowing/backend system to be initialized
 * (i.e. an active Application singleton with a valid native window must exist).
 *
 * @note Mouse position is reported in backend-defined window coordinates.
 *       With GLFW, the origin is the upper-left corner of the client area and
 *       units are screen coordinates (double precision in GLFW, converted to float here).
 */
namespace input {

/**
 * @brief Check whether a key is currently pressed (or repeating).
 * @param key The key to query.
 * @return `true` if the key is down (pressed or repeated), otherwise `false`.
 */
[[nodiscard]] bool is_key_pressed(Key key);

/**
 * @brief Check whether a mouse button is currently pressed.
 * @param button The mouse button to query.
 * @return `true` if the button is down, otherwise `false`.
 */
[[nodiscard]] bool is_mouse_button_pressed(MouseButton button);

/**
 * @brief Get the current mouse cursor position in window coordinates.
 * @return Cursor position as (x, y).
 */
[[nodiscard]] mEn::Vec2 mouse_pos();

/**
 * @brief Show or hide the OS cursor.
 * @param visible If `true`, cursor is visible; if `false`, cursor is hidden.
 *
 * @note Some backends also offer a "disabled/captured" mode for FPS controls.
 *       This function typically maps to a simple visible/hidden state.
 */
void set_cursor_visible(bool visible);

/**
 * @brief Warp the cursor to a specific position (window coordinates).
 * @param pos Target position in window coordinates.
 *
 * @warning Warping the cursor may generate cursor-move events depending on backend.
 */
void set_mouse_pos(mEn::Vec2 pos);

}  // namespace input

}  // namespace kEn