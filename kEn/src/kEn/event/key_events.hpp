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

 private:
  bool is_repeat_;
};

inline std::ostream& operator<<(std::ostream& os, const KeyPressedEvent& e) {
  return os << "KeyPressedEvent: " << e.key() << " (" << key::name_of(e.key())
            << "), mod keys: " << mod_key::active(e.mod_keys()) << ", repeating = " << e.is_repeat();
}

class KeyReleasedEvent : public Event<KeyReleasedEvent>, public KeyEvent {
 public:
  KeyReleasedEvent(const KeyCode& code, const ModKeys& mod) : KeyEvent(code, mod) {}
};

inline std::ostream& operator<<(std::ostream& os, const KeyReleasedEvent& e) {
  return os << "KeyReleasedEvent: " << e.key() << " (" << key::name_of(e.key())
            << "), mod keys: " << mod_key::active(e.mod_keys());
}

class KeyTypedEvent : public Event<KeyTypedEvent>, public KeyEvent {
 public:
  explicit KeyTypedEvent(const KeyCode& code) : KeyEvent(code, 0) {}
};

inline std::ostream& operator<<(std::ostream& os, const KeyTypedEvent& e) {
  return os << "KeyTypedEvent: " << e.key() << " (" << static_cast<char>(e.key()) << ")";
}

}  // namespace kEn