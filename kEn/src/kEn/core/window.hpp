#pragma once

#include <functional>
#include <string>

#include <mEn/vec2.hpp>

#include <kEn/core/core.hpp>
#include <kEn/event/event.hpp>

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

  Window()          = default;
  virtual ~Window() = default;

  virtual void on_update() = 0;

  virtual unsigned int width() const  = 0;
  virtual unsigned int height() const = 0;
  virtual mEn::Vec2 size() const      = 0;

  virtual void set_event_handler(const handler_t& handler) = 0;
  virtual void set_vsync(bool enabled)                     = 0;
  virtual bool vsync() const                               = 0;

  virtual void* native_window() const = 0;

  static Window* create(const WindowProperties& props = WindowProperties());

  VIRTUAL_FIVE(Window);
};

}  // namespace kEn