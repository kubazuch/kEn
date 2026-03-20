#pragma once

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Returns the identity quaternion @f$ (w=1, x=0, y=0, z=0) @f$.
template <typename T>
[[nodiscard]] constexpr qua<T> identity() noexcept;

/// @brief Returns @p q rotated by @p angle radians around axis @p v.
///
/// @pre @p v must be unit-length.
/// @pre @p q should be a unit quaternion.
/// @param q     The quaternion to rotate.
/// @param angle Rotation angle in radians.
/// @param v     Unit rotation axis.
template <typename T>
[[nodiscard]] constexpr qua<T> rotate(const qua<T>& q, T angle, const vec<3, T>& v) noexcept;

/// @brief Returns a unit quaternion representing the rotation that orients along @p direction.
///
/// @param direction Direction to look along.
/// @param up        World up vector (typically (0, 1, 0)).
template <typename T>
[[nodiscard]] constexpr qua<T> quatLookAt(const vec<3, T>& direction, const vec<3, T>& up) noexcept;

}  // namespace mEn

#include "quaternion_transform.inl"
