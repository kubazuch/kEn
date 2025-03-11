#pragma once

#include <kEn/core/key_codes.hpp>
#include <kEn/core/mouse_codes.hpp>
#include <mEn/vec2.hpp>


namespace kEn {

class Input {
 public:
  static bool is_key_pressed(KeyCode key);

  static bool is_mouse_button_pressed(MouseCode button);
  static mEn::Vec2 get_mouse_pos();
  static float get_mouse_x();
  static float get_mouse_y();
  static void set_cursor_visible(bool visible);
  static void set_mouse_pos(mEn::Vec2 pos);
};

}  // namespace kEn