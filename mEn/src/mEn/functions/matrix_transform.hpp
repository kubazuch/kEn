#pragma once

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Returns the N x N identity matrix.
template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> identity() noexcept;

/// @brief Returns @p m translated by vector @p v.
///
/// Equivalent to @f$ m \cdot T(v) @f$ where @f$ T(v) @f$ is the translation matrix.
/// @param m The matrix to translate.
/// @param v Translation vector.
template <typename T>
[[nodiscard]] constexpr mat<4, T> translate(const mat<4, T>& m, const vec<3, T>& v) noexcept;

/// @brief Returns @p m rotated by @p angle radians around axis @p v.
///
/// @pre @p v must be unit-length.
/// @param m     The matrix to rotate.
/// @param angle Rotation angle in radians.
/// @param v     Unit rotation axis.
template <typename T>
[[nodiscard]] constexpr mat<4, T> rotate(const mat<4, T>& m, T angle, const vec<3, T>& v) noexcept;

/// @brief Returns @p m scaled by per-axis factors @p v.
///
/// Equivalent to @f$ m \cdot S(v) @f$ where @f$ S(v) @f$ is the scale matrix.
/// @param m The matrix to scale.
/// @param v Per-axis scale factors (x, y, z).
template <typename T>
[[nodiscard]] constexpr mat<4, T> scale(const mat<4, T>& m, const vec<3, T>& v) noexcept;

/// @brief Returns @p m with a 3D shear transformation applied about pivot @p p.
///
/// @param m   The matrix to shear.
/// @param p   Shear pivot point.
/// @param l_x Shear factors along the X axis: (shear_xy, shear_xz).
/// @param l_y Shear factors along the Y axis: (shear_yx, shear_yz).
/// @param l_z Shear factors along the Z axis: (shear_zx, shear_zy).
template <typename T>
[[nodiscard]] constexpr mat<4, T> shear(const mat<4, T>& m, const vec<3, T>& p, const vec<2, T>& l_x,
                                        const vec<2, T>& l_y, const vec<2, T>& l_z) noexcept;

/// @brief Returns a right-handed view matrix looking from @p eye toward @p center.
///
/// @param eye    Position of the camera.
/// @param center Point the camera is looking at.
/// @param up     World up vector (typically (0, 1, 0)).
template <typename T>
[[nodiscard]] constexpr mat<4, T> lookAt(const vec<3, T>& eye, const vec<3, T>& center, const vec<3, T>& up) noexcept;

}  // namespace mEn

#include "matrix_transform.inl"
