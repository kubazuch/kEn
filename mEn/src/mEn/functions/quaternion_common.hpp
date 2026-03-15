#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

/// @brief Spherical linear blend between unit quaternions @p x and @p y.
///
/// @p t = 0 returns @p x; @p t = 1 returns @p y.
/// Produces constant-angular-velocity interpolation. Does not guarantee shortest arc;
/// use @c slerp for guaranteed shortest-arc interpolation.
/// @pre @p x and @p y should be unit quaternions.
/// @pre @p t must be in [0, 1].
template <typename T>
[[nodiscard]] qua<T> mix(const qua<T>& x, const qua<T>& y, T t) noexcept;

/// @brief Linear (component-wise) blend between quaternions @p x and @p y.
///
/// @p t = 0 returns @p x; @p t = 1 returns @p y.
/// Unlike @c mix/@c slerp, this does not renormalize the result.
/// @pre @p t must be in [0, 1].
template <typename T>
[[nodiscard]] constexpr qua<T> lerp(const qua<T>& x, const qua<T>& y, T t) noexcept;

/// @brief Spherical linear interpolation between unit quaternions @p x and @p y,
/// always along the shortest arc.
///
/// Unlike @c mix, negates @p y when @c dot(x,y) < 0 to guarantee shortest-arc interpolation.
/// @pre @p x and @p y must be unit quaternions.
/// @pre @p t must be in [0, 1].
template <typename T>
[[nodiscard]] qua<T> slerp(const qua<T>& x, const qua<T>& y, T t) noexcept;

/// @brief Returns the conjugate of @p q: @f$ (w, -x, -y, -z) @f$.
///
/// For unit quaternions the conjugate equals the inverse.
template <typename T>
[[nodiscard]] constexpr qua<T> conjugate(const qua<T>& q) noexcept;

/// @brief Returns the multiplicative inverse of @p q: conjugate divided by squared norm.
///
/// For unit quaternions, prefer @c conjugate as it avoids the division.
template <typename T>
[[nodiscard]] constexpr qua<T> inverse(const qua<T>& q) noexcept;

/// @brief Returns a @c vec<4, bool> indicating which components of @p q are NaN.
template <typename T>
[[nodiscard]] vec<4, bool> isnan(const qua<T>& q) noexcept;

/// @brief Returns a @c vec<4, bool> indicating which components of @p q are infinite.
template <typename T>
[[nodiscard]] vec<4, bool> isinf(const qua<T>& q) noexcept;

}  // namespace mEn

#include "quaternion_common.inl"
