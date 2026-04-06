#pragma once

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Returns the rotation angle of unit quaternion @p q in radians.
///
/// The returned angle is in the range @f$ [0, 2\pi] @f$.
template <typename T>
[[nodiscard]] T angle(const qua<T>& q) noexcept;

/// @brief Returns the unit rotation axis of quaternion @p q.
///
/// If @p q is the identity quaternion the returned axis is undefined.
template <typename T>
[[nodiscard]] vec<3, T> axis(const qua<T>& q) noexcept;

/// @brief Constructs a unit quaternion representing a rotation of @p angle radians around @p axis.
///
/// @pre @p axis must be unit-length.
/// @param angle Rotation angle in radians.
/// @param axis  Unit rotation axis.
template <typename T>
[[nodiscard]] qua<T> angleAxis(T angle, const vec<3, T>& axis) noexcept;

/// @brief Returns the Euler angles (pitch, yaw, roll) of unit quaternion @p q in radians.
///
/// The returned @c vec<3> contains @f$ (pitch, yaw, roll) @f$ in that order.
template <typename T>
[[nodiscard]] vec<3, T> eulerAngles(const qua<T>& q) noexcept;

/// @brief Returns the roll angle of quaternion @p q in radians.
///
/// Roll is the rotation around the Z axis.
template <typename T>
[[nodiscard]] T roll(const qua<T>& q) noexcept;

/// @brief Returns the pitch angle of quaternion @p q in radians.
///
/// Pitch is the rotation around the X axis.
template <typename T>
[[nodiscard]] T pitch(const qua<T>& q) noexcept;

/// @brief Returns the yaw angle of quaternion @p q in radians.
///
/// Yaw is the rotation around the Y axis.
template <typename T>
[[nodiscard]] T yaw(const qua<T>& q) noexcept;

}  // namespace mEn

#include "quaternion_trigonometric.inl"
