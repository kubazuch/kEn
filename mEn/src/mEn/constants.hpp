#pragma once

#include <concepts>
#include <limits>

#include <mEn/config.hpp>

namespace mEn {  // NOLINT

template <std::floating_point T = config::default_scalar_t>
inline constexpr T kEpsilon = std::numeric_limits<T>::epsilon();

template <std::floating_point T = config::default_scalar_t>
inline constexpr T kE = std::numbers::e_v<T>;

template <std::floating_point T = config::default_scalar_t>
inline constexpr T kPi = std::numbers::pi_v<T>;

template <std::floating_point T = config::default_scalar_t>
inline constexpr T kHalfPi = std::numbers::pi_v<T> * T{0.5};

template <std::floating_point T = config::default_scalar_t>
inline constexpr T kTwoPi = std::numbers::pi_v<T> * T{2};

template <std::floating_point T = config::default_scalar_t>
inline constexpr T kInvPi = std::numbers::inv_pi_v<T>;

template <std::floating_point T = config::default_scalar_t>
inline constexpr T kInvSqrtPi = std::numbers::inv_sqrtpi_v<T>;

template <std::floating_point T = config::default_scalar_t>
inline constexpr T kNaN = std::numeric_limits<T>::quiet_NaN();

}  // namespace mEn