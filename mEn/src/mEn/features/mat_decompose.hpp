#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

/// @brief Decomposes a 4x4 model matrix into its translation, rotation, and scale components.
///
/// Assumes the matrix has no perspective component (last row must be (0, 0, 0, 1)).
/// On failure, output parameters are left in an unspecified state.
///
/// @param model       Input model matrix to decompose.
/// @param translation Receives the extracted translation vector.
/// @param rotation    Receives the extracted rotation as a unit quaternion.
/// @param scale       Receives the extracted per-axis scale factors.
/// @returns @c true on success, @c false if the matrix cannot be decomposed (perspective component
///          present, near-zero scale, or degenerate matrix).
template <typename T>
[[nodiscard]] bool decompose(const mat<4, T>& model, vec<3, T>& translation, qua<T>& rotation,
                             vec<3, T>& scale) noexcept;

}  // namespace mEn

#include "mat_decompose.inl"
