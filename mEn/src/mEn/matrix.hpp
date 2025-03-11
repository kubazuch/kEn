#pragma once

#include "common.hpp"

// Vectors
#include "vec3.hpp"
#include "vec4.hpp"

// Matrices
#include "mat3.hpp"
#include "mat4.hpp"

namespace mEn {

template <int N, typename T>
[[nodiscard]] constexpr mat<N, T> matrixCompMult(const mat<N, T>& x, const mat<N, T>& y);

template <int N, typename T>
[[nodiscard]] constexpr mat<N, T> outerProduct(const vec<N, T>& c, const vec<N, T>& r);

template <int N, typename T>
[[nodiscard]] constexpr mat<N, T> transpose(const mat<N, T>& m);

template <int N, typename T>
[[nodiscard]] constexpr T determinant(const mat<N, T>& m);

template <int N, typename T>
[[nodiscard]] constexpr mat<N, T> inverse(const mat<N, T>& m);

}  // namespace mEn

#include "matrix.inl"