#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

#include <kEn/util/flags.hpp>

/** @ingroup ken */
namespace kEn {  // NOLINT

/**
 * @brief Modifier key bit flags.
 *
 * Values are intended to be combined as a bitmask (e.g. Shift | Control).
 *
 * @note @ref kEn::util::Flags is used to represent combinations via @ref kEn::ModKeys.
 */
enum class ModKey : std::uint8_t {
  None     = 0U,
  Shift    = static_cast<std::uint8_t>(1U << 0U),
  Control  = static_cast<std::uint8_t>(1U << 1U),
  Alt      = static_cast<std::uint8_t>(1U << 2U),
  Super    = static_cast<std::uint8_t>(1U << 3U),
  CapsLock = static_cast<std::uint8_t>(1U << 4U),
  NumLock  = static_cast<std::uint8_t>(1U << 5U),
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

/// @brief C++20 convenience: brings enumerators into `kEn::mod_key` namespace (e.g. `kEn::mod_key::Control`).
using enum ModKey;

/**
 * @brief Internal mapping of modifier flags to their display names.
 *
 * Used by @ref active() to format active modifiers in a consistent order.
 */
inline constexpr std::array<std::pair<ModKey, std::string_view>, 6> kNames{{
    {ModKey::Shift, "Shift"},
    {ModKey::Control, "Control"},
    {ModKey::Alt, "Alt"},
    {ModKey::Super, "Super"},
    {ModKey::CapsLock, "CapsLock"},
    {ModKey::NumLock, "NumLock"},
}};

/**
 * @brief Get a human-readable name for a single modifier key.
 *
 * @param key Modifier key value.
 * @return A stable string view naming @p key. Returns @c "Unknown" for values not handled.
 *
 * @note This function names single enum values; it does not format combinations.
 *       For combinations, use @ref active().
 */
[[nodiscard]] constexpr std::string_view name(ModKey key) noexcept {
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
    case ModKey::None:
      return "None";
  }
  return "Unknown";
}

/**
 * @brief Get a @ref kEn::ModKeys mask containing all supported modifier flags (excluding None).
 *
 * @return A mask with Shift, Control, Alt, Super, CapsLock, and NumLock set.
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

  for (const auto& [flag, text] : kNames) {
    if (keys.test(flag)) {
      if (!result.empty()) {
        result.push_back(' ');
      }
      result.append(text);
    }
  }
  return result;
}

}  // namespace mod_key

}  // namespace kEn