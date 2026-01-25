#pragma once

#include <cstdint>
#include <string_view>
#include <utility>

/** @ingroup ken */
namespace kEn {  // NOLINT

/**
 * @def MOUSE_CODES(X)
 * @brief X-macro list of mouse button definitions.
 *
 * Expand with `X(id, code)` to generate enum entries and switch cases.
 */
// Copied from "glfw3.h"
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MOUSE_CODES(X) \
  X(button_left, 0)    \
  X(button_right, 1)   \
  X(button_middle, 2)  \
  X(button3, 3)        \
  X(button4, 4)        \
  X(button5, 5)        \
  X(button6, 6)        \
  X(button_last, 7)

/**
 * @brief Strongly-typed mouse button identifier.
 *
 * Values match the corresponding GLFW button codes.
 */
enum class MouseButton : std::uint8_t {
// NOLINTBEGIN(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
#define ENUM_ENTRY(id, code) id = code,
  MOUSE_CODES(ENUM_ENTRY)
#undef ENUM_ENTRY
  // NOLINTEND(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
};

namespace mouse {

/// @brief C++20 convenience: brings enumerators into `kEn::mouse` namespace.
using enum MouseButton;

// NOLINTBEGIN(readability-identifier-naming)
/**
 * @name GLFW-style aliases
 * @brief Compatibility aliases mirroring common `button0..` usage.
 * @{
 */
inline constexpr MouseButton button0 = MouseButton::button_left;
inline constexpr MouseButton button1 = MouseButton::button_right;
inline constexpr MouseButton button2 = MouseButton::button_middle;
inline constexpr MouseButton button7 = MouseButton::button_last;
/** @} */
// NOLINTEND(readability-identifier-naming)

/**
 * @brief Get the underlying numeric mouse button code.
 * @param b Mouse button enum value.
 * @return Numeric button code (matches GLFW).
 */
[[nodiscard]] constexpr auto code(MouseButton b) noexcept { return std::to_underlying(b); }

/**
 * @brief Get a stable, human-readable name for a mouse button.
 * @param b Mouse button enum value.
 * @return A string view describing the button (e.g. `"button_left"`).
 *
 * @note For unknown/unsupported values, returns `"INVALID"`.
 */
[[nodiscard]] constexpr std::string_view name_of(MouseButton b) noexcept {
  // NOLINTBEGIN(cppcoreguidelines-macro-usage)
  switch (b) {
#define CASE_ENTRY(id, code) \
  case MouseButton::id:      \
    return std::string_view{#id};
    MOUSE_CODES(CASE_ENTRY)
#undef CASE_ENTRY
    default:
      return std::string_view{"INVALID"};
  }
  // NOLINTEND(cppcoreguidelines-macro-usage)
}

}  // namespace mouse

}  // namespace kEn

#undef MOUSE_CODES