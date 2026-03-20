// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#pragma once

/** @file
 *  @ingroup ken
 */

#define KEN_EXPAND(X) X
#define KEN_STMT(X) \
  do {                   \
    X                    \
  } while (0)
#define KEN_STRINGIFY(X) #X

#define DELETE_COPY_MOVE(ClassName)                             \
  ClassName(const ClassName&)                     = delete;    \
  ClassName(ClassName&&)                          = delete;    \
  ClassName& operator=(const ClassName&) noexcept = delete;   \
  ClassName& operator=(ClassName&&) noexcept      = delete

template <class>
inline constexpr bool always_false_v = false;  // NOLINT(readability-identifier-naming)

// Single canonical suppression point for the kEn namespace name (mixed-case, intentional).
// Suppressing here avoids repeating NOLINTNEXTLINE in every file that opens namespace kEn.
// NOLINTNEXTLINE(readability-identifier-naming)
namespace kEn {}  // namespace kEn

// NOLINTEND(cppcoreguidelines-macro-usage)