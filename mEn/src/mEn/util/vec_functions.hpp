#pragma once

#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

namespace mEn {

template <int N, typename T>
[[nodiscard]] T length(const vec<N, T>& vec);

template <int N, typename T>
[[nodiscard]] T distance(const vec<N, T>& p0, const vec<N, T>& p1);

template <int N, typename T>
[[nodiscard]] constexpr T dot(const vec<N, T>& u, const vec<N, T>& v);

template <typename T>
[[nodiscard]] constexpr vec<3, T> cross(const vec<3, T>& u, const vec<3, T>& v);

template <int N, typename T>
[[nodiscard]] vec<N, T> normalize(const vec<N, T>& vec);

}  // namespace mEn

#include "vec_functions.inl"