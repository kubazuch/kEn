#pragma once
#include <cstdint>

// Copied from "glfw3.h"
#define MOUSE_CODES(X, Y) \
                          \
  Y(button0, 0)           \
  Y(button1, 1)           \
  Y(button2, 2)           \
  X(button3, 3)           \
  X(button4, 4)           \
  X(button5, 5)           \
  X(button6, 6)           \
  Y(button7, 7)           \
                          \
  X(button_last, 7)       \
  X(button_left, 0)       \
  X(button_right, 1)      \
  X(button_middle, 2)

namespace kEn {

using MouseCode = uint16_t;

namespace mouse {
#define ENUM_ENTRY(id, code) id = (code),
#define CASE_ENTRY(id, code) \
  case code:                 \
    return #id;
#define NO_CASE_ENTRY(id, code)

enum : MouseCode { MOUSE_CODES(ENUM_ENTRY, ENUM_ENTRY) };

inline const char* name_of(const MouseCode key) {
  switch (key) {
    MOUSE_CODES(CASE_ENTRY, NO_CASE_ENTRY)
    default:
      return "INVALID";
  }
}

#undef MOUSE_CODES
#undef ENUM_ENTRY
#undef CASE_ENTRY
#undef NO_CASE_ENTRY
}  // namespace mouse
}  // namespace kEn