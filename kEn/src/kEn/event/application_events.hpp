#pragma once

#include <kEn/event/event.hpp>
#include <ostream>

namespace kEn {

class WindowCloseEvent : public Event<WindowCloseEvent> {
 public:
  WindowCloseEvent() = default;
};

inline std::ostream& operator<<(std::ostream& os, const WindowCloseEvent& /*e*/) { return os << "WindowCloseEvent"; }

class WindowResizeEvent : public Event<WindowResizeEvent> {
 public:
  WindowResizeEvent(unsigned int width, unsigned int height) : width_(width), height_(height) {}

  unsigned int width() const { return width_; }
  unsigned int height() const { return height_; }

 private:
  unsigned int width_, height_;
};

inline std::ostream& operator<<(std::ostream& os, const WindowResizeEvent& e) {
  return os << "WindowResizeEvent: " << e.width() << "x" << e.height();
}

}  // namespace kEn