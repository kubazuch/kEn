#pragma once

#include <kEn/event/event.hpp>
#include <sstream>

namespace kEn {

class WindowCloseEvent : public Event<WindowCloseEvent> {
 public:
  WindowCloseEvent() = default;

  const char* name() const override { return "WindowCloseEvent"; }
};

class WindowResizeEvent : public Event<WindowResizeEvent> {
 public:
  WindowResizeEvent(unsigned int width, unsigned int height) : width_(width), height_(height) {}

  unsigned int width() const { return width_; }
  unsigned int height() const { return height_; }

  const char* name() const override { return "WindowResizeEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << width_ << " x " << height_;
    return ss.str();
  }

 private:
  unsigned int width_, height_;
};

}  // namespace kEn