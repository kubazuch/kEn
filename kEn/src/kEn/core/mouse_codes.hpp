#pragma once

#include <cstdint>
#include <string_view>
#include <type_traits>

namespace kEn {  // NOLINT

/// @brief Underlying integral type used for mouse button codes.
using MouseCode = uint8_t;

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

namespace mouse {

/**
 * @brief Strongly-typed mouse button identifier.
 *
 * Values match the corresponding GLFW button codes.
 */
enum class Button : MouseCode {
// NOLINTBEGIN(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
#define ENUM_ENTRY(id, code) id = code,
  MOUSE_CODES(ENUM_ENTRY)
#undef ENUM_ENTRY
  // NOLINTEND(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
};

/// @brief C++20 convenience: brings enumerators into `kEn::mouse` namespace.
using enum Button;

// NOLINTBEGIN(readability-identifier-naming)
/**
 * @name GLFW-style aliases
 * @brief Compatibility aliases mirroring common `button0..` usage.
 * @{
 */
inline constexpr Button button0 = Button::button_left;
inline constexpr Button button1 = Button::button_right;
inline constexpr Button button2 = Button::button_middle;
inline constexpr Button button7 = Button::button_last;
/** @} */
// NOLINTEND(readability-identifier-naming)

/**
 * @brief Convert an enum value to its underlying integer type.
 * @tparam E Enum type.
 * @param e Enum value.
 * @return Underlying integer representation.
 */
template <class E>
[[nodiscard]] constexpr std::underlying_type_t<E> to_underlying(E e) noexcept {
  return static_cast<std::underlying_type_t<E>>(e);
}

/**
 * @brief Get the underlying numeric mouse button code.
 * @param b Mouse button enum value.
 * @return Numeric button code (matches GLFW).
 */
[[nodiscard]] constexpr MouseCode code(Button b) noexcept { return to_underlying(b); }

/**
 * @brief Get a stable, human-readable name for a mouse button.
 * @param b Mouse button enum value.
 * @return A string view describing the button (e.g. `"button_left"`).
 *
 * @note For unknown/unsupported values, returns `"INVALID"`.
 */
[[nodiscard]] constexpr std::string_view name_of(Button b) noexcept {
  // NOLINTBEGIN(cppcoreguidelines-macro-usage)
  switch (b) {
#define CASE_ENTRY(id, code) \
  case Button::id:           \
    return std::string_view{#id};
    MOUSE_CODES(CASE_ENTRY)
#undef CASE_ENTRY
    default:
      return std::string_view{"INVALID"};
  }
  // NOLINTEND(cppcoreguidelines-macro-usage)
}

/**
 * @brief Get a stable, human-readable name from a raw mouse button code.
 * @param raw Raw numeric button code.
 * @return A string view describing the button, or `"INVALID"` if unknown.
 */
[[nodiscard]] constexpr std::string_view name_of(MouseCode raw) noexcept { return name_of(static_cast<Button>(raw)); }

}  // namespace mouse

}  // namespace kEn

#undef KEN_MOUSE_BUTTON_LIST