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
 * @brief Lock (hide and capture) the cursor for mouse-look controls, or release it.
 *
 * While locked, @ref mouse_pos and mouse-move events report an unbounded virtual position, so movement is read as the
 * difference between successive positions. Uses raw (unaccelerated) motion where supported. Works on platforms that
 * forbid warping the cursor, such as Wayland.
 *
 * @param locked If `true`, lock the cursor; if `false`, restore the normal cursor.
 */
void set_cursor_locked(bool locked);

}  // namespace input

}  // namespace kEn