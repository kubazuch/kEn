#pragma once

#include <mEn/vec2.hpp>

#include <kEn/core/key_codes.hpp>
#include <kEn/core/mouse_codes.hpp>

namespace kEn {

/**
 * @brief Stateless input API for querying keyboard and mouse state.
 *
 * All functions are static and require the windowing/backend system to be
 * initialized (i.e. an active native window handle must exist).
 *
 * @note Mouse position is reported in backend-defined window coordinates.
 *       With GLFW, the origin is the upper-left corner of the client area and
 *       units are screen coordinates (double precision in GLFW, converted to float here).
 */
class Input {
 public:
  /**
   * @brief Check whether a key is currently pressed (or repeating).
   * @param key The key to query.
   * @return `true` if the key is down (pressed or repeated), otherwise `false`.
   */
  [[nodiscard]] static bool is_key_pressed(key::Key key) noexcept;

  /**
   * @brief Check whether a mouse button is currently pressed.
   * @param button The mouse button to query.
   * @return `true` if the button is down, otherwise `false`.
   */
  [[nodiscard]] static bool is_mouse_button_pressed(mouse::Button button) noexcept;

  /**
   * @brief Get the current mouse cursor position in window coordinates.
   * @return Cursor position as (x, y).
   */
  [[nodiscard]] static mEn::Vec2 get_mouse_pos() noexcept;

  /**
   * @brief Get the current mouse cursor X position in window coordinates.
   * @return Cursor x coordinate.
   */
  [[nodiscard]] static float get_mouse_x() noexcept;

  /**
   * @brief Get the current mouse cursor Y position in window coordinates.
   * @return Cursor y coordinate.
   */
  [[nodiscard]] static float get_mouse_y() noexcept;

  /**
   * @brief Show or hide the OS cursor.
   * @param visible If `true`, cursor is visible; if `false`, cursor is hidden.
   *
   * @note Some backends also offer a "disabled/captured" mode for FPS controls.
   *       This function typically maps to a simple visible/hidden state.
   */
  static void set_cursor_visible(bool visible) noexcept;

  /**
   * @brief Warp the cursor to a specific position (window coordinates).
   * @param pos Target position in window coordinates.
   *
   * @warning Warping the cursor may generate cursor-move events depending on backend.
   */
  static void set_mouse_pos(mEn::Vec2 pos) noexcept;
};

}  // namespace kEn