#pragma once

#include <glm/vec2.hpp>
#include <kEn/core/key_codes.hpp>
#include <kEn/core/mouse_codes.hpp>

namespace kEn {

class Input {
 public:
  static bool is_key_pressed(KeyCode key);

  static bool is_mouse_button_pressed(MouseCode button);
  static glm::vec2 get_mouse_pos();
  static float get_mouse_x();
  static float get_mouse_y();
  static void set_cursor_visible(bool visible);
  static void set_mouse_pos(glm::vec2 pos);
};

}  // namespace kEn