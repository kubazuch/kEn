#pragma once

#include <kEn/core/key_codes.hpp>
#include <kEn/core/mod_keys.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class KeyEvent {
 public:
  KeyCode key() const { return key_; }
  ModKeys mod_keys() const { return mod_keys_; }

 protected:
  KeyEvent(const KeyCode& code, const ModKeys& mod) : key_(code), mod_keys_(mod) {}

  KeyCode key_;
  ModKeys mod_keys_;
};

class KeyPressedEvent : public Event<KeyPressedEvent>, public KeyEvent {
 public:
  KeyPressedEvent(const KeyCode& code, const ModKeys& mod, bool repeat = false)
      : KeyEvent(code, mod), is_repeat_(repeat) {}

  bool is_repeat() const { return is_repeat_; }

  const char* name() const override { return "KeyPressedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << key_ << " (" << key::name_of(key_) << "), mod keys: " << mod_key::active(mod_keys_)
       << ", repeating = " << is_repeat_;
    return ss.str();
  }

 private:
  bool is_repeat_;
};

class KeyReleasedEvent : public Event<KeyReleasedEvent>, public KeyEvent {
 public:
  KeyReleasedEvent(const KeyCode& code, const ModKeys& mod) : KeyEvent(code, mod) {}

  const char* name() const override { return "KeyReleasedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << key_ << " (" << key::name_of(key_) << "), mod keys: " << mod_key::active(mod_keys_);
    return ss.str();
  }
};

class KeyTypedEvent : public Event<KeyTypedEvent>, public KeyEvent {
 public:
  explicit KeyTypedEvent(const KeyCode& code) : KeyEvent(code, 0) {}

  const char* name() const override { return "KeyTypedEvent"; }
  std::string to_string() const override {
    std::stringstream ss;
    ss << name() << ": " << key_ << " (" << static_cast<char>(key_) << ")";
    return ss.str();
  }
};

}  // namespace kEn