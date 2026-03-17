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
 * @def KEY_CODES(X)
 * @brief X-macro list of key definitions.
 *
 * Expand with `X(id, code, name)` where:
 * - `id`   -- enumerator identifier (e.g. `space`, `n0`, `escape`)
 * - `code` -- numeric key code
 * - `name` -- human-readable display name (e.g. `"space"`, `"0"`, `"escape"`)
 *
 * Example:
 * @code
 * #define KEY_ENTRY(id, code, name) id = code,
 * KEY_CODES(KEY_ENTRY)
 * #undef KEY_ENTRY
 * @endcode
 */
// Copied from "glfw3.h"
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define KEY_CODES(X)                            \
  /* Printable keys */                          \
  X(space, 32, "space")                         \
  X(apostrophe, 39, "apostrophe") /* ' */       \
  X(comma, 44, "comma")           /* , */       \
  X(minus, 45, "minus")           /* - */       \
  X(period, 46, "period")         /* . */       \
  X(slash, 47, "slash")           /* / */       \
                                                \
  X(n0, 48, "0")                                \
  X(n1, 49, "1")                                \
  X(n2, 50, "2")                                \
  X(n3, 51, "3")                                \
  X(n4, 52, "4")                                \
  X(n5, 53, "5")                                \
  X(n6, 54, "6")                                \
  X(n7, 55, "7")                                \
  X(n8, 56, "8")                                \
  X(n9, 57, "9")                                \
  X(semicolon, 59, "semicolon") /* ; */         \
  X(equals, 61, "equals")       /* = */         \
                                                \
  X(a, 65, "a")                                 \
  X(b, 66, "b")                                 \
  X(c, 67, "c")                                 \
  X(d, 68, "d")                                 \
  X(e, 69, "e")                                 \
  X(f, 70, "f")                                 \
  X(g, 71, "g")                                 \
  X(h, 72, "h")                                 \
  X(i, 73, "i")                                 \
  X(j, 74, "j")                                 \
  X(k, 75, "k")                                 \
  X(l, 76, "l")                                 \
  X(m, 77, "m")                                 \
  X(n, 78, "n")                                 \
  X(o, 79, "o")                                 \
  X(p, 80, "p")                                 \
  X(q, 81, "q")                                 \
  X(r, 82, "r")                                 \
  X(s, 83, "s")                                 \
  X(t, 84, "t")                                 \
  X(u, 85, "u")                                 \
  X(v, 86, "v")                                 \
  X(w, 87, "w")                                 \
  X(x, 88, "x")                                 \
  X(y, 89, "y")                                 \
  X(z, 90, "z")                                 \
                                                \
  X(left_bracket, 91, "left_bracket")   /* [ */ \
  X(backslash, 92, "backslash")         /* \ */ \
  X(right_bracket, 93, "right_bracket") /* ] */ \
  X(grave_accent, 96, "grave_accent")   /* ` */ \
                                                \
  X(world_1, 161, "world_1") /* non-US #1 */    \
  X(world_2, 162, "world_2") /* non-US #2 */    \
                                                \
  /* Function keys */                           \
  X(escape, 256, "escape")                      \
  X(enter, 257, "enter")                        \
  X(tab, 258, "tab")                            \
  X(backspace, 259, "backspace")                \
  X(insert, 260, "insert")                      \
  X(del, 261, "del")                            \
  X(right, 262, "right")                        \
  X(left, 263, "left")                          \
  X(down, 264, "down")                          \
  X(up, 265, "up")                              \
  X(page_up, 266, "page_up")                    \
  X(page_down, 267, "page_down")                \
  X(home, 268, "home")                          \
  X(end, 269, "end")                            \
  X(caps_lock, 280, "caps_lock")                \
  X(scroll_lock, 281, "scroll_lock")            \
  X(num_lock, 282, "num_lock")                  \
  X(print_screen, 283, "print_screen")          \
  X(pause, 284, "pause")                        \
                                                \
  X(f1, 290, "f1")                              \
  X(f2, 291, "f2")                              \
  X(f3, 292, "f3")                              \
  X(f4, 293, "f4")                              \
  X(f5, 294, "f5")                              \
  X(f6, 295, "f6")                              \
  X(f7, 296, "f7")                              \
  X(f8, 297, "f8")                              \
  X(f9, 298, "f9")                              \
  X(f10, 299, "f10")                            \
  X(f11, 300, "f11")                            \
  X(f12, 301, "f12")                            \
  X(f13, 302, "f13")                            \
  X(f14, 303, "f14")                            \
  X(f15, 304, "f15")                            \
  X(f16, 305, "f16")                            \
  X(f17, 306, "f17")                            \
  X(f18, 307, "f18")                            \
  X(f19, 308, "f19")                            \
  X(f20, 309, "f20")                            \
  X(f21, 310, "f21")                            \
  X(f22, 311, "f22")                            \
  X(f23, 312, "f23")                            \
  X(f24, 313, "f24")                            \
  X(f25, 314, "f25")                            \
                                                \
  X(num_0, 320, "num_0")                        \
  X(num_1, 321, "num_1")                        \
  X(num_2, 322, "num_2")                        \
  X(num_3, 323, "num_3")                        \
  X(num_4, 324, "num_4")                        \
  X(num_5, 325, "num_5")                        \
  X(num_6, 326, "num_6")                        \
  X(num_7, 327, "num_7")                        \
  X(num_8, 328, "num_8")                        \
  X(num_9, 329, "num_9")                        \
  X(num_decimal, 330, "num_decimal")            \
  X(num_divide, 331, "num_divide")              \
  X(num_multiply, 332, "num_multiply")          \
  X(num_subtract, 333, "num_subtract")          \
  X(num_add, 334, "num_add")                    \
  X(num_enter, 335, "num_enter")                \
  X(num_equal, 336, "num_equal")                \
                                                \
  X(left_shift, 340, "left_shift")              \
  X(left_control, 341, "left_control")          \
  X(left_alt, 342, "left_alt")                  \
  X(left_super, 343, "left_super")              \
  X(right_shift, 344, "right_shift")            \
  X(right_control, 345, "right_control")        \
  X(right_alt, 346, "right_alt")                \
  X(right_super, 347, "right_super")            \
  X(menu, 348, "menu")

/**
 * @brief Strongly-typed keyboard key identifier.
 *
 * Values match the corresponding key codes.
 * Use `kEn::key::code()` to obtain the underlying numeric code.
 */
enum class Key : std::uint16_t {
// NOLINTBEGIN(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
#define ENUM_ENTRY(id, code, name) id = code,
  KEY_CODES(ENUM_ENTRY)
#undef ENUM_ENTRY
  // NOLINTEND(cppcoreguidelines-macro-usage, bugprone-macro-parentheses, readability-identifier-naming)
};

namespace key {

/// @brief C++20 convenience: brings enumerators into `kEn::key` namespace (e.g. `kEn::key::a`).
using enum Key;

/**
 * @brief Get the underlying numeric key code.
 * @param k Key enum value.
 * @return Numeric key code.
 */
[[nodiscard]] constexpr auto code(Key k) noexcept { return std::to_underlying(k); }

/**
 * @brief Get a stable, human-readable name for a key.
 * @param k Key enum value.
 * @return A string view describing the key (e.g. `"a"`, `"escape"`, `"0"`).
 */
[[nodiscard]] constexpr std::string_view name_of(Key k) noexcept {
  // NOLINTBEGIN(cppcoreguidelines-macro-usage)
  switch (k) {
#define CASE_ENTRY(id, code, name) \
  case Key::id:                    \
    return std::string_view{name};
    KEY_CODES(CASE_ENTRY)
#undef CASE_ENTRY
    default:
      std::unreachable();
  }
  // NOLINTEND(cppcoreguidelines-macro-usage)
}

}  // namespace key

}  // namespace kEn

#undef KEY_CODES
