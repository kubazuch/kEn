#pragma once

#include <ostream>

#include <kEn/core/key_codes.hpp>
#include <kEn/core/mod_keys.hpp>
#include <kEn/event/event.hpp>

/** @ingroup ken */
namespace kEn {

/**
 * @brief Shared payload for keyboard events.
 *
 * Stores the pressed/released/typed key and the active modifier keys at the time of the event.
 * This type is a lightweight helper and does not itself derive from @ref kEn::BaseEvent.
 */
class KeyEvent {
 public:
  /**
   * @brief Get the key associated with the event.
   * @return Key code.
   */
  Key key() const { return key_; }

  /**
   * @brief Get the modifier keys active for the event.
   * @return Modifier key mask.
   */
  ModKeys mod_keys() const { return mod_keys_; }

 protected:
  /**
   * @brief Construct the payload.
   * @param key Key code.
   * @param mod Active modifier keys.
   */
  KeyEvent(const Key& key, const ModKeys& mod) : key_(key), mod_keys_(mod) {}

  /** @brief Key code for the event. */
  Key key_;
  /** @brief Modifier keys active at the time of the event. */
  ModKeys mod_keys_;
};

/**
 * @brief Event fired when a key is pressed.
 *
 * This event may be emitted repeatedly while a key is held down, depending on platform/system
 * key repeat settings.
 */
class KeyPressedEvent : public Event<KeyPressedEvent>, public KeyEvent {
 public:
  /**
   * @brief Construct the event.
   * @param key Key code.
   * @param mod Active modifier keys.
   * @param repeat True if this press is an auto-repeat press while the key is held.
   */
  KeyPressedEvent(const Key& key, const ModKeys& mod, bool repeat = false) : KeyEvent(key, mod), is_repeat_(repeat) {}

  /**
   * @brief Check whether this press is an auto-repeat.
   * @return True if repeated (auto-repeat), false otherwise.
   */
  bool is_repeat() const { return is_repeat_; }

  void write(std::ostream& os) const override {
    os << kName << ": " << key::code(key_) << " (" << key::name_of(key_)
       << "), mod keys: " << mod_key::active(mod_keys_) << ", repeating = " << is_repeat_;
  }

  static constexpr std::string_view kName = "KeyPressedEvent";

 private:
  /** @brief Whether this press originated from key auto-repeat. */
  bool is_repeat_;
};

/**
 * @brief Event fired when a key is released.
 */
class KeyReleasedEvent : public Event<KeyReleasedEvent>, public KeyEvent {
 public:
  /**
   * @brief Construct the event.
   * @param key Key code.
   * @param mod Active modifier keys.
   */
  KeyReleasedEvent(const Key& key, const ModKeys& mod) : KeyEvent(key, mod) {}

  void write(std::ostream& os) const override {
    os << kName << ": " << key::code(key_) << " (" << key::name_of(key_)
       << "), mod keys: " << mod_key::active(mod_keys_);
  }

  static constexpr std::string_view kName = "KeyReleasedEvent";
};

/**
 * @brief Event fired when a key is typed (character input).
 *
 * Unlike press/release events, this typically represents text input semantics.
 * This event is constructed with @ref kEn::ModKey::None for modifiers.
 */
class KeyTypedEvent : public Event<KeyTypedEvent>, public KeyEvent {
 public:
  /**
   * @brief Construct the event.
   * @param key Key code / character source.
   *
   * @note Modifiers are set to @ref kEn::ModKey::None for this event.
   */
  explicit KeyTypedEvent(const Key& key) : KeyEvent(key, ModKey::None) {}

  void write(std::ostream& os) const override {
    os << kName << ": " << key::code(key_) << " (" << static_cast<char>(key_) << ")";
  }

  static constexpr std::string_view kName = "KeyTypedEvent";
};

}  // namespace kEn