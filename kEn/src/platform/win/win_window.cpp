#include "win_window.hpp"

#include <GLFW/glfw3.h>

#include <kEn/core/assert.hpp>
#include <kEn/core/key_codes.hpp>
#include <kEn/core/log.hpp>
#include <kEn/core/mod_keys.hpp>
#include <kEn/core/mouse_codes.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/key_events.hpp>
#include <kEn/event/mouse_events.hpp>
#include <kEn/renderer/graphics_context.hpp>
#include <kEn/renderer/renderer_api.hpp>  // NOLINT

namespace kEn {

namespace {

uint8_t GLFW_window_count = 0;  // NOLINT

void api_error_callback(int error_code, const char* description) {
  KEN_CORE_ERROR("GLFW Error #{0}: {1}", error_code, description);
}

}  // namespace

void WindowsWindow::api_init() {
  const int status = glfwInit();
  KEN_CORE_ASSERT(status, "GLFW init failed!");

  glfwSetErrorCallback(api_error_callback);
}

void WindowsWindow::api_shutdown() { glfwTerminate(); }

Window* Window::create(const WindowProperties& props) { return new WindowsWindow(props); }  // NOLINT

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
    win_data.width  = static_cast<unsigned int>(width);
    win_data.height = static_cast<unsigned int>(height);

    WindowResizeEvent event(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    win_data.handler(event);
  });

  glfwSetKeyCallback(window_ptr_, [](GLFWwindow* window, int key, int /*scancode*/, int action, int mods) {
    Data& win_data       = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    win_data.active_mods = static_cast<ModKeys>(mods);

    switch (action) {  // NOLINT
      case GLFW_PRESS: {
        KeyPressedEvent event(static_cast<KeyCode>(key), static_cast<ModKeys>(mods), false);
        win_data.handler(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(static_cast<KeyCode>(key), static_cast<ModKeys>(mods), true);
        win_data.handler(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(static_cast<KeyCode>(key), static_cast<ModKeys>(mods));
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

  glfwSetMouseButtonCallback(window_ptr_, [](GLFWwindow* window, int button, int action, int mods) {
    Data& win_data       = *static_cast<Data*>(glfwGetWindowUserPointer(window));
    win_data.active_mods = static_cast<ModKeys>(mods);

    double dx, dy;  // NOLINT
    glfwGetCursorPos(window, &dx, &dy);
    auto x = static_cast<float>(dx);
    auto y = static_cast<float>(dy);

    switch (action) {  // NOLINT
      case GLFW_PRESS: {
        MouseButtonPressedEvent event({x, y}, static_cast<MouseCode>(button), static_cast<ModKeys>(mods));
        win_data.handler(event);
        win_data.dragging[button]    = true;
        win_data.drag_from_x[button] = x;
        win_data.drag_from_y[button] = y;
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event({x, y}, static_cast<MouseCode>(button), static_cast<ModKeys>(mods));
        win_data.handler(event);
        win_data.dragging[button] = false;
        break;
      }
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

    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
      if (win_data.dragging[i]) {
        MouseDragEvent drag_event({win_data.drag_from_x[i], win_data.drag_from_y[i]}, {x, y}, static_cast<MouseCode>(i),
                                  win_data.active_mods);
        win_data.handler(drag_event);
      }
    }
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
