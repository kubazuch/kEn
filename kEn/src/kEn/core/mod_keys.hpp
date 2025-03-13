#pragma once

namespace kEn {

using ModKeys = uint8_t;

namespace mod_key {

enum : ModKeys { Shift = 1, Control = 2, Alt = 4, Super = 8, CapsLock = 16, NumLock = 32 };

inline std::string active(const ModKeys mod_keys) {
  std::string result;
  if (mod_keys & Shift) {
    result += "Shift ";
  }
  if (mod_keys & Control) {
    result += "Control ";
  }
  if (mod_keys & Alt) {
    result += "Alt ";
  }
  if (mod_keys & Super) {
    result += "Super ";
  }
  if (mod_keys & CapsLock) {
    result += "CapsLock ";
  }
  if (mod_keys & NumLock) {
    result += "NumLock ";
  }

  if (!result.empty()) {
    result.pop_back();  // Remove the trailing space
  }
  return result;
}

}  // namespace mod_key

}  // namespace kEn