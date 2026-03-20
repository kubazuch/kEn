#pragma once

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#if defined(MEN_USE_GLM) && !defined(MEN_GLM)
#define MEN_GLM 1
#endif

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <numbers>
#include <utility>

#ifdef _MSC_VER
#define MEN_FORCE_INLINE __forceinline
#define MEN_NOINLINE __declspec(noinline)
#else
#define MEN_FORCE_INLINE inline __attribute__((always_inline))
#define MEN_NOINLINE __attribute__((noinline))
#endif

#ifndef MEN_ENABLE_ASSERTS
#ifdef NDEBUG
#define MEN_ENABLE_ASSERTS 0
#else
#define MEN_ENABLE_ASSERTS 1
#endif
#endif

#if MEN_ENABLE_ASSERTS
#define MEN_ASSERT(expr) assert(expr)
#define MEN_UNREACHABLE() (MEN_ASSERT(false), ::std::unreachable())
#else
#define MEN_ASSERT(expr) ((void)0)
#define MEN_UNREACHABLE() ::std::unreachable()
#endif
// NOLINTEND(cppcoreguidelines-macro-usage)

namespace mEn {  // NOLINT(readability-identifier-naming)

namespace config {

#ifdef MEN_GLM
inline constexpr bool use_glm = true;  // NOLINT(readability-identifier-naming)
#else
inline constexpr bool use_glm = false;  // NOLINT(readability-identifier-naming)
#endif

using default_scalar_t = float;

}  // namespace config

}  // namespace mEn
