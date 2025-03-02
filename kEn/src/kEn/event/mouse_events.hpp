#pragma once
#include <kEn/core/mouse_codes.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class mouse_button_event {
 public:
  mouse_code button() const { return button_; }

 protected:
  mouse_button_event(const mouse_code button) : button_(button) {}

  mouse_code button_;
};

class mouse_button_pressed_event : public mouse_button_event, public event<mouse_button_pressed_event> {
 public:
  mouse_button_pressed_event(mouse_code button) : mouse_button_event(button) {}

  const char* name() const override { return "MouseButtonPressedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << button_ << " (" << mouse::name_of(button_) << ")";
    return ss.str();
  }
};

class mouse_button_released_event : public mouse_button_event, public event<mouse_button_released_event> {
 public:
  mouse_button_released_event(mouse_code button) : mouse_button_event(button) {}

  const char* name() const override { return "MouseButtonReleasedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << button_ << " (" << mouse::name_of(button_) << ")";
    return ss.str();
  }
};

class mouse_scroll_event : public event<mouse_scroll_event> {
 public:
  mouse_scroll_event(const float x_offset, const float y_offset) : x_offset_(x_offset), y_offset_(y_offset) {}

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

class mouse_move_event : public event<mouse_move_event> {
 public:
  mouse_move_event(const float x, const float y) : x_pos_(x), y_pos_(y) {}

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
