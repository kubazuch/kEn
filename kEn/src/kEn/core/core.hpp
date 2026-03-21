// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#pragma once

/** @file
 *  @ingroup ken
 */

#define KEN_EXPAND(X) X
#define KEN_STMT(X) \
  do {              \
    X               \
  } while (0)
#define KEN_STRINGIFY(X) #X

#if defined(_MSC_VER)
#define KEN_DEBUGBREAK() __debugbreak()
#elif defined(__clang__)
#define KEN_DEBUGBREAK() __builtin_debugtrap()
#elif defined(__GNUC__)
#define KEN_DEBUGBREAK() __builtin_trap()
#else
#define KEN_DEBUGBREAK() ((void)0)
#endif

#define DELETE_COPY_MOVE(ClassName)                         \
  ClassName(const ClassName&)                     = delete; \
  ClassName(ClassName&&)                          = delete; \
  ClassName& operator=(const ClassName&) noexcept = delete; \
  ClassName& operator=(ClassName&&) noexcept      = delete

template <class>
inline constexpr bool always_false_v = false;  // NOLINT(readability-identifier-naming)

// Single canonical suppression point for the kEn namespace name (mixed-case, intentional).
// Suppressing here avoids repeating NOLINTNEXTLINE in every file that opens namespace kEn.
// NOLINTNEXTLINE(readability-identifier-naming)
namespace kEn {}  // namespace kEn

// NOLINTEND(cppcoreguidelines-macro-usage)