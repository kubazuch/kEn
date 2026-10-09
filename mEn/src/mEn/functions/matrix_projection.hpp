#pragma once

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Returns an orthographic projection matrix.
///
/// Maps the axis-aligned box [@p left, @p right] x [@p bottom, @p top] x [@p zNear, @p zFar]
/// to the OpenGL normalized device coordinate (NDC) cube @f$ [-1, 1]^3 @f$.
/// @param left   Left clipping plane x-coordinate.
/// @param right  Right clipping plane x-coordinate.
/// @param bottom Bottom clipping plane y-coordinate.
/// @param top    Top clipping plane y-coordinate.
/// @param zNear  Near clipping plane distance.
/// @param zFar   Far clipping plane distance.
template <typename T>
[[nodiscard]] constexpr mat<4, T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) noexcept;

/// @brief Returns a perspective projection matrix (right-handed, OpenGL NDC @f$ [-1,1]^3 @f$).
///
/// @param fov    Vertical field of view in radians. Must be positive.
/// @param aspect Viewport width divided by height. Must be positive.
/// @param zNear  Near clipping plane distance. Must be positive.
/// @param zFar   Far clipping plane distance. Must be greater than @p zNear.
template <typename T>
[[nodiscard]] constexpr mat<4, T> perspective(T fov, T aspect, T zNear, T zFar) noexcept;

/// @brief Returns a perspective projection matrix for an arbitrary, possibly off-axis, view frustum
/// (right-handed, OpenGL NDC @f$ [-1,1]^3 @f$).
///
/// The frustum's near-plane rectangle is [@p left, @p right] x [@p bottom, @p top] at distance @p zNear.
/// A symmetric rectangle gives the same matrix as @ref perspective; a horizontally shifted one gives the
/// off-axis projection used for stereoscopic rendering.
/// @param left   Left edge of the near-plane rectangle.
/// @param right  Right edge of the near-plane rectangle.
/// @param bottom Bottom edge of the near-plane rectangle.
/// @param top    Top edge of the near-plane rectangle.
/// @param zNear  Near clipping plane distance. Must be positive.
/// @param zFar   Far clipping plane distance. Must be greater than @p zNear.
template <typename T>
[[nodiscard]] constexpr mat<4, T> frustum(T left, T right, T bottom, T top, T zNear, T zFar) noexcept;

}  // namespace mEn

#include "matrix_projection.inl"
