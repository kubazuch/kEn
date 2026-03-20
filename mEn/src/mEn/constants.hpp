#pragma once

#include <concepts>
#include <limits>

#include <mEn/config.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief A small value suitable for practical floating-point comparisons.
///
/// Used to test whether values are "close enough to zero" for geometric
/// operations (e.g. degenerate vectors, near-zero lengths). Intentionally
/// larger than machine epsilon to tolerate single-precision accumulation.
/// @tparam T Floating-point type. Defaults to @c config::default_scalar_t.
template <std::floating_point T = config::default_scalar_t>
inline constexpr T kEpsilon = T{1e-6};

/// @brief Euler's number @f$ e \approx 2.71828 @f$.
/// @tparam T Floating-point type. Defaults to @c config::default_scalar_t.
template <std::floating_point T = config::default_scalar_t>
inline constexpr T kE = std::numbers::e_v<T>;

/// @brief The mathematical constant @f$ \pi \approx 3.14159 @f$.
/// @tparam T Floating-point type. Defaults to @c config::default_scalar_t.
template <std::floating_point T = config::default_scalar_t>
inline constexpr T kPi = std::numbers::pi_v<T>;

/// @brief Half of @f$ \pi @f$: @f$ \pi / 2 \approx 1.5708 @f$.
///
/// Equivalent to a right angle in radians.
/// @tparam T Floating-point type. Defaults to @c config::default_scalar_t.
template <std::floating_point T = config::default_scalar_t>
inline constexpr T kHalfPi = std::numbers::pi_v<T> * T{0.5};

/// @brief Two times @f$ \pi @f$: @f$ 2\pi \approx 6.2832 @f$.
///
/// Equivalent to a full rotation in radians.
/// @tparam T Floating-point type. Defaults to @c config::default_scalar_t.
template <std::floating_point T = config::default_scalar_t>
inline constexpr T kTwoPi = std::numbers::pi_v<T> * T{2};

/// @brief Reciprocal of @f$ \pi @f$: @f$ 1 / \pi \approx 0.31831 @f$.
/// @tparam T Floating-point type. Defaults to @c config::default_scalar_t.
template <std::floating_point T = config::default_scalar_t>
inline constexpr T kInvPi = std::numbers::inv_pi_v<T>;

/// @brief Reciprocal of the square root of @f$ \pi @f$:
/// @f$ 1 / \sqrt{\pi} \approx 0.56419 @f$.
/// @tparam T Floating-point type. Defaults to @c config::default_scalar_t.
template <std::floating_point T = config::default_scalar_t>
inline constexpr T kInvSqrtPi = std::numbers::inv_sqrtpi_v<T>;

/// @brief Quiet NaN (Not a Number) for floating-point type @p T.
///
/// Useful as a sentinel value. Note: any comparison with NaN returns false,
/// including @c kNaN == kNaN.
/// @tparam T Floating-point type. Defaults to @c config::default_scalar_t.
template <std::floating_point T = config::default_scalar_t>
inline constexpr T kNaN = std::numeric_limits<T>::quiet_NaN();

}  // namespace mEn
