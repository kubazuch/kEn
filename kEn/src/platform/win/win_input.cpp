#include <GLFW/glfw3.h>

#include <kEn/core/application.hpp>
#include <kEn/core/input.hpp>
#include <kenpch.hpp>

namespace kEn {

bool Input::is_key_pressed(const KeyCode key) {
  auto* window         = static_cast<GLFWwindow*>(Application::instance().main_window().native_window());
  const auto key_state = glfwGetKey(window, key);
  return key_state == GLFW_PRESS;
}

bool Input::is_mouse_button_pressed(const MouseCode button) {
  auto* window            = static_cast<GLFWwindow*>(Application::instance().main_window().native_window());
  const auto button_state = glfwGetMouseButton(window, button);
  return button_state == GLFW_PRESS;
}

mEn::Vec2 Input::get_mouse_pos() {
  auto* window = static_cast<GLFWwindow*>(Application::instance().main_window().native_window());
  double x     = 0.0;
  double y     = 0.0;
  glfwGetCursorPos(window, &x, &y);

  return {static_cast<float>(x), static_cast<float>(y)};
}

float Input::get_mouse_x() { return get_mouse_pos().x; }

float Input::get_mouse_y() { return get_mouse_pos().y; }

void Input::set_cursor_visible(bool visible) {
  auto* window = static_cast<GLFWwindow*>(Application::instance().main_window().native_window());
  glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void Input::set_mouse_pos(mEn::Vec2 pos) {
  auto* window = static_cast<GLFWwindow*>(Application::instance().main_window().native_window());
  glfwSetCursorPos(window, pos.x, pos.y);
}

}  // namespace kEn