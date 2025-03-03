#include <kEn/core/assert.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/key_events.hpp>
#include <kEn/event/mouse_events.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kenpch.hpp>
#include <platform/win/win_window.hpp>

#include "kEn/renderer/graphics_context.hpp"

namespace kEn {

static uint8_t GLFW_window_count = 0;

static void api_error_callback(int error_code, const char* description) {
  KEN_CORE_ERROR("GLFW Error #{0}: {1}", error_code, description);
}

void WindowsWindow::api_init() {
  const int status = glfwInit();
  KEN_CORE_ASSERT(status, "GLFW init failed!");

  glfwSetErrorCallback(api_error_callback);
}

void WindowsWindow::api_shutdown() { glfwTerminate(); }

Window* Window::create(const WindowProperties& props) { return new WindowsWindow(props); }

WindowsWindow::WindowsWindow(const WindowProperties& properties) {
  data_.title  = properties.title;
  data_.width  = properties.width;
  data_.height = properties.height;

  KEN_CORE_DEBUG("Creating window {0} ({1} x {2})", properties.title, properties.width, properties.height);

  if (GLFW_window_count == 0) {
    api_init();
  }

#ifdef _KEN_DEBUG
  if (RendererApi::get_api() == RendererApi::Api::OpenGL) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  }
#endif
  glfwWindowHint(GLFW_SAMPLES, 4);

  window_ptr_ = glfwCreateWindow(static_cast<int>(data_.width), static_cast<int>(data_.height), data_.title.c_str(),
                                 nullptr, nullptr);
  ++GLFW_window_count;

  context_ = GraphicsContext::create(window_ptr_);
  context_->init();

  glfwSetWindowUserPointer(window_ptr_, &data_);
  WindowsWindow::set_vsync(true);

  set_glfw_callbacks();
}

void WindowsWindow::set_glfw_callbacks() const {
  // events purr!
  glfwSetWindowCloseCallback(window_ptr_, [](GLFWwindow* window) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    WindowCloseEvent event;
    win_data.handler(event);
  });

  glfwSetWindowSizeCallback(window_ptr_, [](GLFWwindow* window, int width, int height) {
    Data& win_data  = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    win_data.width  = width;
    win_data.height = height;

    WindowResizeEvent event(width, height);
    win_data.handler(event);
  });

  glfwSetKeyCallback(window_ptr_, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(static_cast<KeyCode>(key), false);
        win_data.handler(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(static_cast<KeyCode>(key), true);
        win_data.handler(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(static_cast<KeyCode>(key));
        win_data.handler(event);
        break;
      }
    }
  });

  glfwSetCharCallback(window_ptr_, [](GLFWwindow* window, unsigned int keycode) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    KeyTypedEvent event(static_cast<KeyCode>(keycode));
    win_data.handler(event);
  });

  glfwSetMouseButtonCallback(window_ptr_, [](GLFWwindow* window, int button, int action, int /*mods*/) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

    switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(static_cast<MouseCode>(button));
        win_data.handler(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
        win_data.handler(event);
        break;
      }
    }
  });

  glfwSetScrollCallback(window_ptr_, [](GLFWwindow* window, double x_offset, double y_offset) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

    MouseScrollEvent event(static_cast<float>(x_offset), static_cast<float>(y_offset));
    win_data.handler(event);
  });

  glfwSetCursorPosCallback(window_ptr_, [](GLFWwindow* window, double x, double y) {
    const Data& win_data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

    MouseMoveEvent event(static_cast<float>(x), static_cast<float>(y));
    win_data.handler(event);
  });
}

WindowsWindow::~WindowsWindow() {
  glfwDestroyWindow(window_ptr_);
  --GLFW_window_count;

  if (GLFW_window_count == 0) {
    api_shutdown();
  }
}

void WindowsWindow::on_update() {
  glfwPollEvents();
  context_->swap_buffers();
}

void WindowsWindow::set_vsync(const bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  data_.vsync = enabled;
}

bool WindowsWindow::vsync() const { return data_.vsync; }

}  // namespace kEn
