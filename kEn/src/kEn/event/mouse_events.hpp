#pragma once

#include <mEn/features/format.hpp>
#include <mEn/vec2.hpp>

#include <kEn/core/mod_keys.hpp>
#include <kEn/core/mouse_codes.hpp>
#include <kEn/event/event.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Shared payload for mouse button events.
 *
 * Stores the mouse button involved and the active modifier keys at the time of the event.
 * This type is a lightweight helper and does not itself derive from @ref kEn::BaseEvent.
 */
class MouseButtonEvent {
 public:
  /**
   * @brief Get the mouse button associated with the event.
   * @return Mouse button code.
   */
  [[nodiscard]] MouseButton button() const { return button_; }
  /**
   * @brief Get the modifier keys active for the event.
   * @return Modifier key mask.
   */
  [[nodiscard]] ModKeys mod_keys() const { return mod_keys_; }

 protected:
  /**
   * @brief Construct the payload.
   * @param button Mouse button code.
   * @param mod Active modifier keys.
   */
  MouseButtonEvent(MouseButton button, ModKeys mod) : button_(button), mod_keys_(mod) {}

 private:
  /** @brief Mouse button for the event. */
  MouseButton button_;
  /** @brief Modifier keys active at the time of the event. */
  ModKeys mod_keys_;
};

/**
 * @brief Shared payload for mouse button events that include a cursor position.
 *
 * Extends @ref MouseButtonEvent with a cursor position, eliminating the duplication
 * between @ref MouseButtonPressedEvent and @ref MouseButtonReleasedEvent.
 */
class MouseButtonActionEvent : public MouseButtonEvent {
 public:
  /**
   * @brief Get the cursor position for the event.
   * @return Cursor position.
   */
  [[nodiscard]] const mEn::Vec2& pos() const { return pos_; }

 protected:
  /**
   * @brief Construct the payload.
   * @param pos Cursor position.
   * @param button Mouse button code.
   * @param mod Active modifier keys.
   */
  MouseButtonActionEvent(const mEn::Vec2& pos, MouseButton button, ModKeys mod)
      : MouseButtonEvent(button, mod), pos_(pos) {}

 private:
  /** @brief Cursor position at the time of the event. */
  mEn::Vec2 pos_;
};

/**
 * @brief Event fired when a mouse button is pressed.
 *
 * Includes cursor position at the time of the press.
 */
class MouseButtonPressedEvent : public MouseButtonActionEvent, public Event<MouseButtonPressedEvent> {
 public:
  /**
   * @brief Construct the event.
   * @param pos Cursor position.
   * @param button Mouse button code.
   * @param mod Active modifier keys.
   */
  MouseButtonPressedEvent(const mEn::Vec2& pos, MouseButton button, ModKeys mod)
      : MouseButtonActionEvent(pos, button, mod) {}

  [[nodiscard]] std::string to_string() const override {
    return std::format("{}: {}, button {} ({}), mod keys: {}", kName, pos(), mouse::code(button()),
                       mouse::name_of(button()), mod_key::active(mod_keys()));
  }

  static constexpr std::string_view kName = "MouseButtonPressedEvent";
};

/**
 * @brief Event fired when a mouse button is released.
 *
 * Includes cursor position at the time of the release.
 */
class MouseButtonReleasedEvent : public MouseButtonActionEvent, public Event<MouseButtonReleasedEvent> {
 public:
  /**
   * @brief Construct the event.
   * @param pos Cursor position.
   * @param button Mouse button code.
   * @param mod Active modifier keys.
   */
  MouseButtonReleasedEvent(const mEn::Vec2& pos, MouseButton button, ModKeys mod)
      : MouseButtonActionEvent(pos, button, mod) {}

  [[nodiscard]] std::string to_string() const override {
    return std::format("{}: {}, button {} ({}), mod keys: {}", kName, pos(), mouse::code(button()),
                       mouse::name_of(button()), mod_key::active(mod_keys()));
  }

  static constexpr std::string_view kName = "MouseButtonReleasedEvent";
};

/**
 * @brief Event fired when the mouse wheel/scroll input is used.
 *
 * Carries a 2D scroll offset (commonly x = horizontal, y = vertical).
 */
class MouseScrollEvent : public Event<MouseScrollEvent> {
 public:
  /**
   * @brief Construct the event.
   * @param offset Scroll offset.
   */
  explicit MouseScrollEvent(const mEn::Vec2& offset) : offset_(offset) {}

  /**
   * @brief Get the scroll offset.
   * @return Scroll offset.
   */
  [[nodiscard]] const mEn::Vec2& offset() const { return offset_; }

  [[nodiscard]] std::string to_string() const override { return std::format("{}: {}", kName, offset_); }

  static constexpr std::string_view kName = "MouseScrollEvent";

 private:
  /** @brief Scroll offset value. */
  mEn::Vec2 offset_;
};

/**
 * @brief Event fired when the mouse cursor moves.
 */
class MouseMoveEvent : public Event<MouseMoveEvent> {
 public:
  /**
   * @brief Construct the event.
   * @param pos New cursor position.
   */
  explicit MouseMoveEvent(const mEn::Vec2& pos) : pos_(pos) {}

  /**
   * @brief Get the current cursor position.
   * @return Cursor position.
   */
  [[nodiscard]] const mEn::Vec2& pos() const { return pos_; }

  [[nodiscard]] std::string to_string() const override { return std::format("{}: {}", kName, pos_); }

  static constexpr std::string_view kName = "MouseMoveEvent";

 private:
  /** @brief Cursor position. */
  mEn::Vec2 pos_;
};

/**
 * @brief Event fired when the mouse is dragged while a button is held.
 *
 * Includes start and end positions for the drag segment, the button being held,
 * and active modifier keys.
 */
class MouseDragEvent : public MouseButtonEvent, public Event<MouseDragEvent> {
 public:
  /**
   * @brief Construct the event.
   * @param from Drag start position.
   * @param to Drag end position.
   * @param button Mouse button code being held.
   * @param mod Active modifier keys.
   */
  MouseDragEvent(const mEn::Vec2& from, const mEn::Vec2& to, MouseButton button, ModKeys mod)
      : MouseButtonEvent(button, mod), from_(from), to_(to) {}

  /**
   * @brief Get the drag start position.
   * @return Start position.
   */
  [[nodiscard]] const mEn::Vec2& from() const { return from_; }
  /**
   * @brief Get the drag end position.
   * @return End position.
   */
  [[nodiscard]] const mEn::Vec2& to() const { return to_; }

  [[nodiscard]] std::string to_string() const override {
    return std::format("{}: {} to {}, button {} ({}), mod keys: {}", kName, from_, to_, mouse::code(button()),
                       mouse::name_of(button()), mod_key::active(mod_keys()));
  }

  static constexpr std::string_view kName = "MouseDragEvent";

 private:
  /** @brief Drag start position. */
  mEn::Vec2 from_;
  /** @brief Drag end position. */
  mEn::Vec2 to_;
};

}  // namespace kEn
