#pragma once

#include <cstdint>
#include <string_view>
#include <utility>

/** @file
 *  @ingroup ken
 */

namespace kEn {  // NOLINT

/**
 * @def KEY_CODES(X, Y)
 * @brief X-macro list of key definitions.
 *
 * Expand with:
 * - `X(id, code)` for entries where the display name is the identifier
 * - `Y(id, code, name)` for entries that need a custom display name (e.g. digits)
 *
 * Example:
 * @code
 * #define ENUM_ENTRY(id, code) id = code,
 * #define NAMED_ENUM_ENTRY(id, code, name) id = code,
 * KEY_CODES(ENUM_ENTRY, NAMED_ENUM_ENTRY)
 * @endcode
 */
// Copied from "glfw3.h"
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define KEY_CODES(X, Y)           \
  /* Printable keys */            \
  X(space, 32)                    \
  X(apostrophe, 39) /* ' */       \
  X(comma, 44)      /* , */       \
  X(minus, 45)      /* - */       \
  X(period, 46)     /* . */       \
  X(slash, 47)      /* / */       \
                                  \
  Y(_0, 48, "0")                  \
  Y(_1, 49, "1")                  \
  Y(_2, 50, "2")                  \
  Y(_3, 51, "3")                  \
  Y(_4, 52, "4")                  \
  Y(_5, 53, "5")                  \
  Y(_6, 54, "6")                  \
  Y(_7, 55, "7")                  \
  Y(_8, 56, "8")                  \
  Y(_9, 57, "9")                  \
  X(semicolon, 59) /* ; */        \
  X(equals, 61)    /* = */        \
                                  \
  X(a, 65)                        \
  X(b, 66)                        \
  X(c, 67)                        \
  X(d, 68)                        \
  X(e, 69)                        \
  X(f, 70)                        \
  X(g, 71)                        \
  X(h, 72)                        \
  X(i, 73)                        \
  X(j, 74)                        \
  X(k, 75)                        \
  X(l, 76)                        \
  X(m, 77)                        \
  X(n, 78)                        \
  X(o, 79)                        \
  X(p, 80)                        \
  X(q, 81)                        \
  X(r, 82)                        \
  X(s, 83)                        \
  X(t, 84)                        \
  X(u, 85)                        \
  X(v, 86)                        \
  X(w, 87)                        \
  X(x, 88)                        \
  X(y, 89)                        \
  X(z, 90)                        \
                                  \
  X(left_bracket, 91)  /* [ */    \
  X(backslash, 92)     /* \ */    \
  X(right_bracket, 93) /* ] */    \
  X(grave_accent, 96)  /* ` */    \
                                  \
  X(world_1, 161) /* non-US #1 */ \
  X(world_2, 162) /* non-US #2 */ \
                                  \
  /* Function keys */             \
  X(escape, 256)                  \
  X(enter, 257)                   \
  X(tab, 258)                     \
  X(backspace, 259)               \
  X(insert, 260)                  \
  X(del, 261)                     \
  X(right, 262)                   \
  X(left, 263)                    \
  X(down, 264)                    \
  X(up, 265)                      \
  X(page_up, 266)                 \
  X(page_down, 267)               \
  X(home, 268)                    \
  X(end, 269)                     \
  X(caps_lock, 280)               \
  X(scroll_lock, 281)             \
  X(num_lock, 282)                \
  X(print_screen, 283)            \
  X(pause, 284)                   \
                                  \
  X(f1, 290)                      \
  X(f2, 291)                      \
  X(f3, 292)                      \
  X(f4, 293)                      \
  X(f5, 294)                      \
  X(f6, 295)                      \
  X(f7, 296)                      \
  X(f8, 297)                      \
  X(f9, 298)                      \
  X(f10, 299)                     \
  X(f11, 300)                     \
  X(f12, 301)                     \
  X(f13, 302)                     \
  X(f14, 303)                     \
  X(f15, 304)                     \
  X(f16, 305)                     \
  X(f17, 306)                     \
  X(f18, 307)                     \
  X(f19, 308)                     \
  X(f20, 309)                     \
  X(f21, 310)                     \
  X(f22, 311)                     \
  X(f23, 312)                     \
  X(f24, 313)                     \
  X(f25, 314)                     \
                                  \
  X(num_0, 320)                   \
  X(num_1, 321)                   \
  X(num_2, 322)                   \
  X(num_3, 323)                   \
  X(num_4, 324)                   \
  X(num_5, 325)                   \
  X(num_6, 326)                   \
  X(num_7, 327)                   \
  X(num_8, 328)                   \
  X(num_9, 329)                   \
  X(num_decimal, 330)             \
  X(num_divide, 331)              \
  X(num_multiply, 332)            \
  X(num_subtract, 333)            \
  X(num_add, 334)                 \
  X(num_enter, 335)               \
  X(num_equal, 336)               \
                                  \
  X(left_shift, 340)              \
  X(left_control, 341)            \
  X(left_alt, 342)                \
  X(left_super, 343)              \
  X(right_shift, 344)             \
  X(right_control, 345)           \
  X(right_alt, 346)               \
  X(right_super, 347)             \
  X(menu, 348)

/**
 * @brief Strongly-typed keyboard key identifier.
 *
 * Values match the corresponding GLFW key codes.
 * Use `kEn::key::code()` to obtain the underlying numeric code.
 */
enum class Key : std::uint16_t {
// NOLINTBEGIN(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
#define ENUM_ENTRY(id, code) id = code,
#define NAMED_ENUM_ENTRY(id, code, name) id = code,
  KEY_CODES(ENUM_ENTRY, NAMED_ENUM_ENTRY)
#undef ENUM_ENTRY
#undef NAMED_ENUM_ENTRY
  // NOLINTEND(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
};

namespace key {

/// @brief C++20 convenience: brings enumerators into `kEn::key` namespace (e.g. `kEn::key::a`).
using enum Key;

/**
 * @brief Get the underlying numeric key code.
 * @param k Key enum value.
 * @return Numeric key code (matches GLFW).
 */
[[nodiscard]] constexpr auto code(Key k) noexcept { return std::to_underlying(k); }

/**
 * @brief Get a stable, human-readable name for a key.
 * @param k Key enum value.
 * @return A string view describing the key (e.g. `"a"`, `"escape"`, `"0"`).
 *
 * @note For unknown/unsupported values, returns `"INVALID"`.
 */
[[nodiscard]] constexpr std::string_view name_of(Key k) noexcept {
  // NOLINTBEGIN(cppcoreguidelines-macro-usage)
  switch (k) {
#define CASE_ENTRY(id, code) \
  case Key::id:              \
    return std::string_view{#id};
#define NAMED_CASE_ENTRY(id, code, name) \
  case Key::id:                          \
    return std::string_view{name};
    KEY_CODES(CASE_ENTRY, NAMED_CASE_ENTRY)
#undef CASE_ENTRY
#undef NAMED_CASE_ENTRY
    default:
      return std::string_view{"INVALID"};
  }
  // NOLINTEND(cppcoreguidelines-macro-usage)
}

}  // namespace key

}  // namespace kEn

#undef KEY_CODES