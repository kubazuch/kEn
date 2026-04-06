#include "input.hpp"

#include <GLFW/glfw3.h>

#include <mEn/fwd.hpp>
#include <mEn/vec2.hpp>

#include <kEn/core/application.hpp>
#include <kEn/core/input/key_codes.hpp>
#include <kEn/core/input/mouse_codes.hpp>

namespace kEn::input {

namespace {

[[nodiscard]] GLFWwindow* glfw_window() {
  // Assume main window is constant for the whole lifetime of app
  static GLFWwindow* const kWindow = Application::instance().main_window().native_window();
  return kWindow;
}

}  // namespace

bool is_key_pressed(Key key) {
  auto* window         = glfw_window();
  const auto key_state = glfwGetKey(window, static_cast<int>(key));
  return key_state == GLFW_PRESS || key_state == GLFW_REPEAT;
}

bool is_mouse_button_pressed(MouseButton button) {
  auto* window            = glfw_window();
  const auto button_state = glfwGetMouseButton(window, static_cast<int>(button));
  return button_state == GLFW_PRESS;
}

mEn::Vec2 mouse_pos() {
  auto* window = glfw_window();
  double x     = 0.0;
  double y     = 0.0;
  glfwGetCursorPos(window, &x, &y);

  return {static_cast<float>(x), static_cast<float>(y)};
}

void set_cursor_visible(bool visible) {
  auto* window = glfw_window();
  glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void set_mouse_pos(mEn::Vec2 pos) {
  auto* window = glfw_window();
  glfwSetCursorPos(window, static_cast<double>(pos.x), static_cast<double>(pos.y));
}

}  // namespace kEn::input
