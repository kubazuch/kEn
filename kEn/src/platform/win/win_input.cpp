#include <GLFW/glfw3.h>

#include <kEn/core/application.hpp>
#include <kEn/core/input.hpp>
#include <kenpch.hpp>

namespace kEn {

bool input::is_key_pressed(const key_code key) {
  auto* window         = static_cast<GLFWwindow*>(application::instance().main_window().native_window());
  const auto key_state = glfwGetKey(window, key);
  return key_state == GLFW_PRESS;
}

bool input::is_mouse_button_pressed(const mouse_code button) {
  auto* window            = static_cast<GLFWwindow*>(application::instance().main_window().native_window());
  const auto button_state = glfwGetMouseButton(window, button);
  return button_state == GLFW_PRESS;
}

glm::vec2 input::get_mouse_pos() {
  auto* window = static_cast<GLFWwindow*>(application::instance().main_window().native_window());
  double x, y;
  glfwGetCursorPos(window, &x, &y);

  return {static_cast<float>(x), static_cast<float>(y)};
}

float input::get_mouse_x() { return get_mouse_pos().x; }

float input::get_mouse_y() { return get_mouse_pos().y; }

void input::set_cursor_visible(bool visible) {
  auto* window = static_cast<GLFWwindow*>(application::instance().main_window().native_window());
  glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void input::set_mouse_pos(glm::vec2 pos) {
  auto* window = static_cast<GLFWwindow*>(application::instance().main_window().native_window());
  glfwSetCursorPos(window, pos.x, pos.y);
}

}  // namespace kEn