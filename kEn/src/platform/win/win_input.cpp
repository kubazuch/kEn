#include <GLFW/glfw3.h>

#include <mEn/vec2.hpp>

#include <kEn/core/application.hpp>
#include <kEn/core/input.hpp>
#include <kEn/core/key_codes.hpp>
#include <kEn/core/mouse_codes.hpp>

namespace kEn {

namespace {
[[nodiscard]] GLFWwindow* glfw_window() noexcept {
  return static_cast<GLFWwindow*>(Application::instance().main_window().native_window());
}

}  // namespace

bool Input::is_key_pressed(key::Key key) noexcept {
  auto* window         = glfw_window();
  const auto key_state = glfwGetKey(window, static_cast<int>(key));
  return key_state == GLFW_PRESS || key_state == GLFW_REPEAT;
}

bool Input::is_mouse_button_pressed(mouse::Button button) noexcept {
  auto* window            = glfw_window();
  const auto button_state = glfwGetMouseButton(window, static_cast<int>(button));
  return button_state == GLFW_PRESS;
}

mEn::Vec2 Input::get_mouse_pos() noexcept {
  auto* window = glfw_window();
  double x     = 0.0;
  double y     = 0.0;
  glfwGetCursorPos(window, &x, &y);

  return {static_cast<float>(x), static_cast<float>(y)};
}

float Input::get_mouse_x() noexcept { return get_mouse_pos().x; }

float Input::get_mouse_y() noexcept { return get_mouse_pos().y; }

void Input::set_cursor_visible(bool visible) noexcept {
  auto* window = glfw_window();
  glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void Input::set_mouse_pos(mEn::Vec2 pos) noexcept {
  auto* window = glfw_window();
  glfwSetCursorPos(window, pos.x, pos.y);
}

}  // namespace kEn