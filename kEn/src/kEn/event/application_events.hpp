#pragma once

#include <ostream>

#include <kEn/event/event.hpp>

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
  WindowResizeEvent(unsigned int width, unsigned int height) : width_(width), height_(height) {}

  /**
   * @brief Get the new window width in pixels.
   * @return Width in pixels.
   */
  unsigned int width() const { return width_; }
  /**
   * @brief Get the new window height in pixels.
   * @return Height in pixels.
   */
  unsigned int height() const { return height_; }

  void write(std::ostream& os) const override { os << kName << ": " << width() << "x" << height(); }

  static constexpr std::string_view kName = "WindowResizeEvent";

 private:
  /** @brief New window dimensions (pixels). */
  unsigned int width_, height_;
};

}  // namespace kEn