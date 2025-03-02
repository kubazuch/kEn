#pragma once

#include <GLFW/glfw3.h>

#include <kEn/core/core.hpp>
#include <kEn/core/window.hpp>
#include <kEn/renderer/graphics_context.hpp>

namespace kEn {

class windows_window : public window {
 public:
  windows_window(const window_properties& properties);
  virtual ~windows_window() override;

  void on_update() override;

  inline unsigned int width() const override { return data_.width; }
  inline unsigned int height() const override { return data_.height; }

  inline void set_event_handler(const handler_t& handler) override { data_.handler = handler; }

  void set_vsync(bool enabled) override;
  bool vsync() const override;

  void* native_window() const override { return window_ptr; }

  VIRTUAL_FIVE(windows_window);

 private:
  static void api_init();
  static void api_shutdown();

 private:
  void set_glfw_callbacks() const;

  GLFWwindow* window_ptr;
  std::unique_ptr<graphics_context> context_;

  struct data {
    std::string title;
    unsigned int width, height;

    bool vsync;

    handler_t handler;
  };

  data data_;
};

}  // namespace kEn
