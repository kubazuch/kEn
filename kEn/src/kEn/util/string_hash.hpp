#pragma once

#include <cstddef>
#include <string_view>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Transparent hash functor for string-keyed associative containers.
 *
 * Enables heterogeneous lookup in containers declared as:
 * @code
 * std::unordered_map<std::string, V, StringHash, std::equal_to<>>
 * @endcode
 *
 * Accepts @c const char*, @c std::string_view, and @c std::string without
 * constructing a temporary @c std::string for the lookup key.
 */
struct StringHash {
  using is_transparent = void;

  [[nodiscard]] std::size_t operator()(std::string_view txt) const { return std::hash<std::string_view>{}(txt); }
};

}  // namespace kEn
