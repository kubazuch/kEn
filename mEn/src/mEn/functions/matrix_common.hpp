#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> matrixCompMult(const mat<N, T>& x, const mat<N, T>& y) noexcept;

template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> outerProduct(const vec<N, T>& c, const vec<N, T>& r) noexcept;

template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> transpose(const mat<N, T>& m) noexcept;

template <length_t N, typename T>
[[nodiscard]] constexpr T determinant(const mat<N, T>& m) noexcept;

template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> inverse(const mat<N, T>& m) noexcept;

}  // namespace mEn

#include "matrix_common.inl"
