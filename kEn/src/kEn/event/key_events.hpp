#pragma once

#include <kEn/core/key_codes.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class KeyEvent {
 public:
  KeyCode key() const { return key_; }

 protected:
  explicit KeyEvent(const KeyCode code) : key_(code) {}

  KeyCode key_;
};

class KeyPressedEvent : public Event<KeyPressedEvent>, public KeyEvent {
 public:
  explicit KeyPressedEvent(const KeyCode code, bool repeat = false) : KeyEvent(code), is_repeat_(repeat) {}

  bool is_repeat() const { return is_repeat_; }

  const char* name() const override { return "KeyPressedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << key_ << " (" << key::name_of(key_) << "), repeating = " << is_repeat_;
    return ss.str();
  }

 private:
  bool is_repeat_;
};

class KeyReleasedEvent : public Event<KeyReleasedEvent>, public KeyEvent {
 public:
  explicit KeyReleasedEvent(const KeyCode code) : KeyEvent(code) {}

  const char* name() const override { return "KeyReleasedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << key_ << " (" << key::name_of(key_) << ")";
    return ss.str();
  }
};

class KeyTypedEvent : public Event<KeyTypedEvent>, public KeyEvent {
 public:
  explicit KeyTypedEvent(const KeyCode code) : KeyEvent(code) {}

  const char* name() const override { return "KeyTypedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << key_ << " (" << static_cast<char>(key_) << ")";
    return ss.str();
  }
};

}  // namespace kEn