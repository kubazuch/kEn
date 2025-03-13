#pragma once
#include <kEn/core/mod_keys.hpp>
#include <kEn/core/mouse_codes.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

// TODO: rework
class MouseButtonEvent {
 public:
  MouseCode button() const { return button_; }
  ModKeys mod_keys() const { return mod_keys_; }

 protected:
  MouseButtonEvent(const MouseCode& button, const ModKeys& mod) : button_(button), mod_keys_(mod) {}

  MouseCode button_;
  ModKeys mod_keys_;
};

class MouseButtonPressedEvent : public MouseButtonEvent, public Event<MouseButtonPressedEvent> {
 public:
  explicit MouseButtonPressedEvent(float x, float y, const MouseCode& button, const ModKeys& mod)
      : MouseButtonEvent(button, mod), x_pos_(x), y_pos_(y) {}

  float x() const { return x_pos_; }
  float y() const { return y_pos_; }

  const char* name() const override { return "MouseButtonPressedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << x_pos_ << ", " << y_pos_ << ", button " << button_ << " (" << mouse::name_of(button_)
       << "), mod keys: " << mod_key::active(mod_keys_);
    return ss.str();
  }

 private:
  float x_pos_, y_pos_;
};

class MouseButtonReleasedEvent : public MouseButtonEvent, public Event<MouseButtonReleasedEvent> {
 public:
  explicit MouseButtonReleasedEvent(float x, float y, const MouseCode& button, const ModKeys& mod)
      : MouseButtonEvent(button, mod), x_pos_(x), y_pos_(y) {}

  float x() const { return x_pos_; }
  float y() const { return y_pos_; }

  const char* name() const override { return "MouseButtonReleasedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << x_pos_ << ", " << y_pos_ << ", button " << button_ << " (" << mouse::name_of(button_)
       << "), mod keys: " << mod_key::active(mod_keys_);
    return ss.str();
  }

 private:
  float x_pos_, y_pos_;
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
  MouseMoveEvent(float x, float y) : x_pos_(x), y_pos_(y) {}

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

class MouseDragEvent : public Event<MouseDragEvent>, public MouseButtonEvent {
 public:
  MouseDragEvent(float from_x, float from_y, float to_x, float to_y, const MouseCode& button, const ModKeys& mod)
      : MouseButtonEvent(button, mod), from_x_(from_x), from_y_(from_y), to_x_(to_x), to_y_(to_y) {}

  float from_x() const { return from_x_; }
  float from_y() const { return from_y_; }
  float to_x() const { return to_x_; }
  float to_y() const { return to_y_; }

  const char* name() const override { return "MouseDragEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": from " << from_x_ << ", " << from_y_ << ", to " << to_x_ << ", " << to_y_ << ", button "
       << button_ << " (" << mouse::name_of(button_) << "), mod keys: " << mod_key::active(mod_keys_);
    return ss.str();
  }

 private:
  float from_x_, from_y_;
  float to_x_, to_y_;
};

}  // namespace kEn
