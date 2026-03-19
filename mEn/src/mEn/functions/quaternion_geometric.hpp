#pragma once

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Returns the norm (magnitude) of quaternion @p q: @f$ \|q\| = \sqrt{w^2+x^2+y^2+z^2} @f$.
template <typename T>
[[nodiscard]] T length(const qua<T>& q) noexcept;

/// @brief Returns a unit-length quaternion in the direction of @p q.
///
/// If @p q has zero length, returns the identity quaternion @f$ (w=1, x=0, y=0, z=0) @f$.
template <typename T>
[[nodiscard]] qua<T> normalize(const qua<T>& q) noexcept;

/// @brief Returns the 4-component dot product: @f$ x \cdot y = w_xw_y + x_xx_y + y_xy_y + z_xz_y @f$.
template <typename T>
[[nodiscard]] constexpr T dot(const qua<T>& x, const qua<T>& y) noexcept;

/// @brief Returns the Hamilton product @f$ x \otimes y @f$ (alias for @c operator*).
///
/// Note: quaternion multiplication is not commutative; @f$ x \otimes y \neq y \otimes x @f$ in general.
template <typename T>
[[nodiscard]] constexpr qua<T> cross(const qua<T>& x, const qua<T>& y) noexcept;

}  // namespace mEn

#include "quaternion_geometric.inl"
