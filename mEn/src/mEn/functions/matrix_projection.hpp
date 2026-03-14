#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <typename T>
[[nodiscard]] constexpr mat<4, T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) noexcept;

template <typename T>
[[nodiscard]] constexpr mat<4, T> perspective(T fov, T aspect, T zNear, T zFar) noexcept;

}  // namespace mEn

#include "matrix_projection.inl"
