#pragma once

#include <mEn/vec2.hpp>

#include <kEn/core/mod_keys.hpp>
#include <kEn/core/mouse_codes.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

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
  MouseButtonPressedEvent(const mEn::Vec2& pos, const MouseCode& button, const ModKeys& mod)
      : MouseButtonEvent(button, mod), pos_(pos) {}

  const mEn::Vec2& pos() const { return pos_; }

 private:
  mEn::Vec2 pos_;
};

inline std::ostream& operator<<(std::ostream& os, const MouseButtonPressedEvent& e) {
  return os << "MouseButtonPressedEvent: " << e.pos() << ", button " << e.button() << " (" << mouse::name_of(e.button())
            << "), mod keys: " << mod_key::active(e.mod_keys());
}

class MouseButtonReleasedEvent : public MouseButtonEvent, public Event<MouseButtonReleasedEvent> {
 public:
  MouseButtonReleasedEvent(const mEn::Vec2& pos, const MouseCode& button, const ModKeys& mod)
      : MouseButtonEvent(button, mod), pos_(pos) {}

  const mEn::Vec2& pos() const { return pos_; }

 private:
  mEn::Vec2 pos_;
};

inline std::ostream& operator<<(std::ostream& os, const MouseButtonReleasedEvent& e) {
  return os << "MouseButtonReleasedEvent: " << e.pos() << ", button " << e.button() << " ("
            << mouse::name_of(e.button()) << "), mod keys: " << mod_key::active(e.mod_keys());
}

class MouseScrollEvent : public Event<MouseScrollEvent> {
 public:
  explicit MouseScrollEvent(const mEn::Vec2 offset) : offset_(offset) {}

  const mEn::Vec2& offset() const { return offset_; }

 private:
  mEn::Vec2 offset_;
};

inline std::ostream& operator<<(std::ostream& os, const MouseScrollEvent& e) {
  return os << "MouseScrollEvent: " << e.offset();
}

class MouseMoveEvent : public Event<MouseMoveEvent> {
 public:
  explicit MouseMoveEvent(const mEn::Vec2 pos) : pos_(pos) {}

  const mEn::Vec2& pos() const { return pos_; }

 private:
  mEn::Vec2 pos_;
};

inline std::ostream& operator<<(std::ostream& os, const MouseMoveEvent& e) {
  return os << "MouseMoveEvent: " << e.pos();
}

class MouseDragEvent : public MouseButtonEvent, public Event<MouseDragEvent> {
 public:
  MouseDragEvent(const mEn::Vec2& from, const mEn::Vec2& to, const MouseCode& button, const ModKeys& mod)
      : MouseButtonEvent(button, mod), from_(from), to_(to) {}

  const mEn::Vec2& from() const { return from_; }
  const mEn::Vec2& to() const { return to_; }

 private:
  mEn::Vec2 from_, to_;
};

inline std::ostream& operator<<(std::ostream& os, const MouseDragEvent& e) {
  return os << "MouseDragEvent: from " << e.from() << " to " << e.to() << ", button " << e.button() << " ("
            << mouse::name_of(e.button()) << "), mod keys: " << mod_key::active(e.mod_keys());
}

}  // namespace kEn
