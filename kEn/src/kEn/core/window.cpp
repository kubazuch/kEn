#include "window.hpp"

#include <GLFW/glfw3.h>

#include <cstddef>
#include <cstdint>

#include <kEn/core/assert.hpp>
#include <kEn/core/input/key_codes.hpp>
#include <kEn/core/input/mouse_codes.hpp>
#include <kEn/core/log.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/key_events.hpp>
#include <kEn/event/mouse_events.hpp>

namespace kEn {

uint8_t Window::glfw_window_count_ = 0;

namespace {

void api_error_callback(int error_code, const char* description) {
  KEN_CORE_ERROR("GLFW Error #{0}: {1}", error_code, description);
}

void api_init() {
  [[maybe_unused]] const int status = glfwInit();
  KEN_CORE_ASSERT(status, "GLFW init failed!");

  glfwSetErrorCallback(api_error_callback);
}

void api_shutdown() { glfwTerminate(); }

}  // namespace

Window::Window(const WindowProperties& properties) {
  data_.title  = properties.title;
  data_.width  = properties.width;
  data_.height = properties.height;

  KEN_CORE_DEBUG("Creating window {0} ({1} x {2})", properties.title, properties.width, properties.height);

  if (glfw_window_count_ == 0) {
    api_init();
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  if (properties.opengl_debug_context) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  }

  window_ptr_ = glfwCreateWindow(static_cast<int>(data_.width), static_cast<int>(data_.height), data_.title.c_str(),
                                 nullptr, nullptr);
  KEN_CORE_ASSERT(window_ptr_, "glfwCreateWindow failed!");
  ++glfw_window_count_;

  glfwSetWindowUserPointer(window_ptr_, &data_);

  set_glfw_callbacks();
}

Window::~Window() {
  glfwDestroyWindow(window_ptr_);
  --glfw_window_count_;

  if (glfw_window_count_ == 0) {
    api_shutdown();
  }
}

void Window::set_glfw_callbacks() {
  // events purr!
  glfwSetWindowCloseCallback(window_ptr_, [](GLFWwindow* window) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    WindowCloseEvent event;
    win_data.handler(event);
  });

  glfwSetWindowSizeCallback(window_ptr_, [](GLFWwindow* window, int width, int height) {
    Data& win_data  = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    win_data.width  = static_cast<std::uint32_t>(width);
    win_data.height = static_cast<std::uint32_t>(height);

    WindowResizeEvent event(static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height));
    win_data.handler(event);
  });

  glfwSetKeyCallback(window_ptr_, [](GLFWwindow* window, int key, int /*scancode*/, int action, int mods) {
    Data& win_data       = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    win_data.active_mods = static_cast<ModKeys>(static_cast<ModKeys::underlying_type>(mods));

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(static_cast<Key>(key), win_data.active_mods, false);
        win_data.handler(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(static_cast<Key>(key), win_data.active_mods, true);
        win_data.handler(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(static_cast<Key>(key), win_data.active_mods);
        win_data.handler(event);
        break;
      }
      default:
        break;
    }
  });

  glfwSetCharCallback(window_ptr_, [](GLFWwindow* window, unsigned int keycode) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    KeyTypedEvent event(static_cast<Key>(keycode));
    win_data.handler(event);
  });

  glfwSetMouseButtonCallback(window_ptr_, [](GLFWwindow* window, int button, int action, int mods) {
    Data& win_data       = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    win_data.active_mods = static_cast<ModKeys>(static_cast<ModKeys::underlying_type>(mods));

    double dx{};
    double dy{};
    glfwGetCursorPos(window, &dx, &dy);
    auto x = static_cast<float>(dx);
    auto y = static_cast<float>(dy);

    switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event({x, y}, static_cast<MouseButton>(button), win_data.active_mods);
        win_data.handler(event);
        win_data.drag_state[static_cast<std::size_t>(button)] = {.active = true, .from = {x, y}};
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event({x, y}, static_cast<MouseButton>(button), win_data.active_mods);
        win_data.handler(event);
        win_data.drag_state[static_cast<std::size_t>(button)].active = false;
        break;
      }
      default:
        break;
    }
  });

  glfwSetScrollCallback(window_ptr_, [](GLFWwindow* window, double x_offset, double y_offset) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

    MouseScrollEvent event({x_offset, y_offset});
    win_data.handler(event);
  });

  glfwSetCursorPosCallback(window_ptr_, [](GLFWwindow* window, double x, double y) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

    MouseMoveEvent event({x, y});
    win_data.handler(event);

    for (std::size_t i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
      if (win_data.drag_state[i].active) {
        MouseDragEvent drag_event(win_data.drag_state[i].from, {x, y}, static_cast<MouseButton>(i),
                                  win_data.active_mods);
        win_data.handler(drag_event);
      }
    }
  });
}

void Window::poll_events() { glfwPollEvents(); }  // NOLINT(readability-convert-member-functions-to-static)

void Window::set_vsync(const bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  data_.vsync = enabled;
}

bool Window::vsync() const { return data_.vsync; }

}  // namespace kEn
