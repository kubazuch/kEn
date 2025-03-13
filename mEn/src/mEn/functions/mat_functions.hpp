#pragma once

#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>

namespace mEn {

template <int N, typename T>
[[nodiscard]] constexpr mat<N, T> identity();

template <typename T>
[[nodiscard]] constexpr mat<4, T> translate(const mat<4, T>& m, const vec<3, T>& v);

template <typename T>
[[nodiscard]] mat<4, T> scale(const mat<4, T>& m, const vec<3, T>& v);

}  // namespace mEn

#include "mat_functions.inl"