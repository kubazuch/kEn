#pragma once

#include <cstdint>
#include <string_view>
#include <utility>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @def MOUSE_CODES(X)
 * @brief X-macro list of mouse button definitions.
 *
 * Expand with `X(id, code, name)` where:
 * - `id`   -- enumerator identifier (e.g. `button_left`, `button4`)
 * - `code` -- numeric button code (0-based)
 * - `name` -- human-readable display name
 *
 * Example:
 * @code
 * #define BUTTON_ENTRY(id, code, name) id = code,
 * MOUSE_CODES(BUTTON_ENTRY)
 * #undef BUTTON_ENTRY
 * @endcode
 */
// Copied from "glfw3.h"
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MOUSE_CODES(X)                 \
  X(button_left, 0, "button_left")     \
  X(button_right, 1, "button_right")   \
  X(button_middle, 2, "button_middle") \
  X(button4, 3, "button4")             \
  X(button5, 4, "button5")             \
  X(button6, 5, "button6")             \
  X(button7, 6, "button7")             \
  X(button8, 7, "button8")

/**
 * @brief Strongly-typed mouse button identifier.
 *
 * Values match the corresponding button codes (0-based).
 * Use `kEn::mouse::code()` to obtain the underlying numeric code.
 */
enum class MouseButton : std::uint8_t {
// NOLINTBEGIN(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
#define ENUM_ENTRY(id, code, name) id = code,
  MOUSE_CODES(ENUM_ENTRY)
#undef ENUM_ENTRY
  // NOLINTEND(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
};

namespace mouse {

/// @brief C++20 convenience: brings enumerators into `kEn::mouse` namespace.
using enum MouseButton;

// NOLINTBEGIN(readability-identifier-naming)
/**
 * @name Numeric aliases
 * @brief Aliases for the named buttons.
 * @{
 */
inline constexpr MouseButton button1 = MouseButton::button_left;
inline constexpr MouseButton button2 = MouseButton::button_right;
inline constexpr MouseButton button3 = MouseButton::button_middle;
/** @} */
// NOLINTEND(readability-identifier-naming)

/**
 * @brief Get the underlying numeric mouse button code.
 * @param b Mouse button enum value.
 * @return Numeric button code.
 */
[[nodiscard]] constexpr auto code(MouseButton b) noexcept { return std::to_underlying(b); }

/**
 * @brief Get a stable, human-readable name for a mouse button.
 * @param b Mouse button enum value.
 * @return A string view describing the button (e.g. `"button_left"`).
 */
[[nodiscard]] constexpr std::string_view name_of(MouseButton b) noexcept {
  // NOLINTBEGIN(cppcoreguidelines-macro-usage)
  switch (b) {
#define CASE_ENTRY(id, code, name) \
  case MouseButton::id:            \
    return std::string_view{name};
    MOUSE_CODES(CASE_ENTRY)
#undef CASE_ENTRY
    default:
      std::unreachable();
  }
  // NOLINTEND(cppcoreguidelines-macro-usage)
}

}  // namespace mouse

}  // namespace kEn

#undef MOUSE_CODES
