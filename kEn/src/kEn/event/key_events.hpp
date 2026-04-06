#pragma once

#include <format>
#include <utility>

#include <kEn/core/input/key_codes.hpp>
#include <kEn/core/input/mod_keys.hpp>
#include <kEn/event/event.hpp>

/** @file
 *  @ingroup ken
 */

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
  [[nodiscard]] Key key() const { return key_; }

  /**
   * @brief Get the modifier keys active for the event.
   * @return Modifier key mask.
   */
  [[nodiscard]] ModKeys mod_keys() const { return mod_keys_; }

 protected:
  /**
   * @brief Construct the payload.
   * @param key Key code.
   * @param mod Active modifier keys.
   */
  KeyEvent(Key key, ModKeys mod) : key_(key), mod_keys_(mod) {}

 private:
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
  KeyPressedEvent(Key key, ModKeys mod, bool repeat = false) : KeyEvent(key, mod), is_repeat_(repeat) {}

  /**
   * @brief Check whether this press is an auto-repeat.
   * @return True if repeated (auto-repeat), false otherwise.
   */
  [[nodiscard]] bool is_repeat() const { return is_repeat_; }

  [[nodiscard]] std::string to_string() const override {
    return std::format("{}: {} ({}), mod keys: {}, repeating = {}", kName, key::code(key()), key::name_of(key()),
                       mod_key::active(mod_keys()), is_repeat_);
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
  KeyReleasedEvent(Key key, ModKeys mod) : KeyEvent(key, mod) {}

  [[nodiscard]] std::string to_string() const override {
    return std::format("{}: {} ({}), mod keys: {}", kName, key::code(key()), key::name_of(key()),
                       mod_key::active(mod_keys()));
  }

  static constexpr std::string_view kName = "KeyReleasedEvent";
};

/**
 * @brief Event fired when a key is typed (character input).
 *
 * Unlike press/release events, this typically represents text input semantics.
 * This event carries no modifier state; modifiers are always empty.
 */
class KeyTypedEvent : public Event<KeyTypedEvent>, public KeyEvent {
 public:
  /**
   * @brief Construct the event.
   * @param key Key code / character source.
   */
  explicit KeyTypedEvent(Key key) : KeyEvent(key, ModKeys{}) {}

  [[nodiscard]] std::string to_string() const override {
    return std::format("{}: {} ({})", kName, key::code(key()),
                       static_cast<char>(static_cast<unsigned char>(std::to_underlying(key()))));
  }

  static constexpr std::string_view kName = "KeyTypedEvent";
};

}  // namespace kEn
