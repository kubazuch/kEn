#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

#include <kEn/core/core.hpp>
#include <kEn/util/flags.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Modifier key bit flags.
 *
 * Values are intended to be combined as a bitmask (e.g. Shift | Control).
 *
 * @note @ref kEn::util::Flags is used to represent combinations via @ref kEn::ModKeys.
 */
enum class ModKey : std::uint8_t {
  Shift    = 1U << 0,
  Control  = 1U << 1,
  Alt      = 1U << 2,
  Super    = 1U << 3,
  CapsLock = 1U << 4,
  NumLock  = 1U << 5,
};

/**
 * @brief Enable flag operators for @ref kEn::ModKey.
 *
 * After this, expressions like @c ModKey::Shift | ModKey::Control produce a
 * @ref kEn::ModKeys value.
 */
KEN_ENABLE_FLAGS(::kEn::ModKey);

/**
 * @brief Flag-mask type representing a set of modifier keys.
 *
 * This is a type-safe bitmask wrapper around @ref kEn::ModKey.
 */
using ModKeys = util::Flags<ModKey>;

namespace mod_key {

/** @brief C++20 convenience: brings enumerators into `kEn::mod_key` namespace (e.g. `kEn::mod_key::Control`). */
using enum ModKey;

/**
 * @brief Get a human-readable name for a single modifier key.
 *
 * @param key Modifier key value.
 * @return A stable string view naming @p key.
 *
 * @note This function names single enum values; it does not format combinations.
 *       For combinations, use @ref kEn::mod_key::active.
 * @note Passing a value not in @ref kEn::ModKey is undefined behaviour (`std::unreachable`).
 */
[[nodiscard]] constexpr std::string_view name_of(ModKey key) noexcept {
  switch (key) {
    case ModKey::Shift:
      return "Shift";
    case ModKey::Control:
      return "Control";
    case ModKey::Alt:
      return "Alt";
    case ModKey::Super:
      return "Super";
    case ModKey::CapsLock:
      return "CapsLock";
    case ModKey::NumLock:
      return "NumLock";
    default:
      std::unreachable();
  }
}

/**
 * @brief Get a @ref kEn::ModKeys mask containing all supported modifier flags (excluding None).
 *
 * @return A mask with all six modifier flags set.
 */
[[nodiscard]] constexpr ModKeys all() noexcept {
  return ModKey::Shift | ModKey::Control | ModKey::Alt | ModKey::Super | ModKey::CapsLock | ModKey::NumLock;
}

/**
 * @brief Format the active modifiers as a space-separated string.
 *
 * The output is built in a deterministic order (Shift, Control, Alt, Super, CapsLock, NumLock).
 *
 * @param keys Modifier mask to format.
 * @return If @p keys is empty, returns @c "None". Otherwise returns a space-separated list
 *         of names corresponding to set flags.
 */
[[nodiscard]] inline std::string active(ModKeys keys) {
  std::string result;
  result.reserve(64);

  if (keys.none()) {
    result = "None";
    return result;
  }

  static constexpr std::array<ModKey, 6> kOrder{{
      ModKey::Shift,
      ModKey::Control,
      ModKey::Alt,
      ModKey::Super,
      ModKey::CapsLock,
      ModKey::NumLock,
  }};

  for (ModKey flag : kOrder) {
    if (keys.test(flag)) {
      if (!result.empty()) {
        result.push_back(' ');
      }
      result.append(name_of(flag));
    }
  }
  return result;
}

}  // namespace mod_key

}  // namespace kEn
