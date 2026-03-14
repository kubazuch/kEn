#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> identity() noexcept;

template <typename T>
[[nodiscard]] constexpr mat<4, T> translate(const mat<4, T>& m, const vec<3, T>& v) noexcept;

template <typename T>
[[nodiscard]] constexpr mat<4, T> rotate(const mat<4, T>& m, T angle, const vec<3, T>& v) noexcept;

template <typename T>
[[nodiscard]] constexpr mat<4, T> scale(const mat<4, T>& m, const vec<3, T>& v) noexcept;

template <typename T>
[[nodiscard]] constexpr mat<4, T> shear(const mat<4, T>& m, const vec<3, T>& p, const vec<2, T>& l_x,
                                        const vec<2, T>& l_y, const vec<2, T>& l_z) noexcept;

template <typename T>
[[nodiscard]] constexpr mat<4, T> lookAt(const vec<3, T>& eye, const vec<3, T>& center, const vec<3, T>& up) noexcept;

}  // namespace mEn

#include "matrix_transform.inl"
