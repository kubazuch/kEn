#pragma once
#include <kEn/core/mouse_codes.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class MouseButtonEvent {
 public:
  MouseCode button() const { return button_; }

 protected:
  explicit MouseButtonEvent(const MouseCode button) : button_(button) {}

  MouseCode button_;
};

class MouseButtonPressedEvent : public MouseButtonEvent, public Event<MouseButtonPressedEvent> {
 public:
  explicit MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {}

  const char* name() const override { return "MouseButtonPressedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << button_ << " (" << mouse::name_of(button_) << ")";
    return ss.str();
  }
};

class MouseButtonReleasedEvent : public MouseButtonEvent, public Event<MouseButtonReleasedEvent> {
 public:
  explicit MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}

  const char* name() const override { return "MouseButtonReleasedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << button_ << " (" << mouse::name_of(button_) << ")";
    return ss.str();
  }
};

class MouseScrollEvent : public Event<MouseScrollEvent> {
 public:
  MouseScrollEvent(const float x_offset, const float y_offset) : x_offset_(x_offset), y_offset_(y_offset) {}

  float x_offset() const { return x_offset_; }
  float y_offset() const { return y_offset_; }

  const char* name() const override { return "MouseScrollEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << x_offset_ << ", " << y_offset_;
    return ss.str();
  }

 private:
  float x_offset_, y_offset_;
};

class MouseMoveEvent : public Event<MouseMoveEvent> {
 public:
  MouseMoveEvent(const float x, const float y) : x_pos_(x), y_pos_(y) {}

  float x() const { return x_pos_; }
  float y() const { return y_pos_; }

  const char* name() const override { return "MouseMoveEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << x_pos_ << ", " << y_pos_;
    return ss.str();
  }

 private:
  float x_pos_, y_pos_;
};

}  // namespace kEn
