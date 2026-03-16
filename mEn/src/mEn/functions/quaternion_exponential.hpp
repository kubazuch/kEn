#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

/// @brief Returns the quaternion exponential @f$ e^q @f$.
///
/// For a pure quaternion @f$ q = (0, v) @f$,
/// @f$ e^q = (\cos\|v\|,\ \sin\|v\| \cdot v/\|v\|) @f$.
template <typename T>
[[nodiscard]] qua<T> exp(const qua<T>& q) noexcept;

/// @brief Returns the quaternion natural logarithm @f$ \ln(q) @f$.
///
/// @pre @p q must be a unit quaternion.
template <typename T>
[[nodiscard]] qua<T> log(const qua<T>& q) noexcept;

/// @brief Returns the quaternion @p q raised to the real power @p x: @f$ q^x = e^{x \ln q} @f$.
///
/// @pre @p q must be a unit quaternion.
template <typename T>
[[nodiscard]] qua<T> pow(const qua<T>& q, T x) noexcept;

/// @brief Returns the square root of @p q: @f$ q^{1/2} @f$.
///
/// @pre @p q must be a unit quaternion.
template <typename T>
[[nodiscard]] qua<T> sqrt(const qua<T>& q) noexcept;

}  // namespace mEn

#include "quaternion_exponential.inl"
