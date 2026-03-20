#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <functional>
#include <memory>
#include <string>

#include <mEn/vec2.hpp>

#include <kEn/core/mod_keys.hpp>
#include <kEn/event/event.hpp>
#include <kEn/renderer/graphics_context.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

struct WindowProperties {
  std::string title;
  unsigned int width;
  unsigned int height;

  explicit WindowProperties(std::string title = "kEngine", unsigned int width = 1280, unsigned int height = 720)
      : title(std::move(title)), width(width), height(height) {}
};

class Window {
 public:
  using handler_t = std::function<void(BaseEvent&)>;

  explicit Window(const WindowProperties& properties = WindowProperties());
  ~Window();

  DELETE_COPY_MOVE(Window);

  void on_update();

  [[nodiscard]] unsigned int width() const { return data_.width; }
  [[nodiscard]] unsigned int height() const { return data_.height; }
  [[nodiscard]] mEn::Vec2 size() const { return {data_.width, data_.height}; }

  void set_event_handler(handler_t handler) { data_.handler = std::move(handler); }
  void set_vsync(bool enabled);
  [[nodiscard]] bool vsync() const;

  [[nodiscard]] GLFWwindow* native_window() const { return window_ptr_; }

 private:
  void set_glfw_callbacks();

  static uint8_t glfw_window_count_;

  GLFWwindow* window_ptr_;
  std::unique_ptr<GraphicsContext> context_;

  struct DragState {
    bool active{};
    mEn::Vec2 from{};
  };

  struct Data {
    std::string title;
    unsigned int width{}, height{};

    bool vsync{};

    std::array<DragState, GLFW_MOUSE_BUTTON_LAST> drag_state{};
    ModKeys active_mods;

    handler_t handler;
  };

  Data data_;
};

}  // namespace kEn
