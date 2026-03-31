#pragma once

#include <cstdint>
#include <format>

#include <kEn/event/event.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Event fired when the window is requested to close.
 *
 * This is typically emitted in response to the user pressing the window close button or the OS
 * initiating a close request.
 */
class WindowCloseEvent : public Event<WindowCloseEvent> {
 public:
  /** @brief Construct the event. */
  WindowCloseEvent() = default;

  static constexpr std::string_view kName = "WindowCloseEvent";
};

/**
 * @brief Event fired when the window is resized.
 *
 * Carries the new window dimensions in pixels.
 */
class WindowResizeEvent : public Event<WindowResizeEvent> {
 public:
  /**
   * @brief Construct the event.
   * @param width New window width in pixels.
   * @param height New window height in pixels.
   */
  WindowResizeEvent(std::uint32_t width, std::uint32_t height) : width_(width), height_(height) {}

  /**
   * @brief Get the new window width in pixels.
   * @return Width in pixels.
   */
  [[nodiscard]] std::uint32_t width() const { return width_; }
  /**
   * @brief Get the new window height in pixels.
   * @return Height in pixels.
   */
  [[nodiscard]] std::uint32_t height() const { return height_; }

  [[nodiscard]] std::string to_string() const override { return std::format("{}: {}x{}", kName, width(), height()); }

  static constexpr std::string_view kName = "WindowResizeEvent";

 private:
  std::uint32_t width_;
  std::uint32_t height_;
};

}  // namespace kEn