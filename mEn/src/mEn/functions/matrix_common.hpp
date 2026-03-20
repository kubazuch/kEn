#pragma once

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Returns the component-wise (Hadamard) product of matrices @p x and @p y.
///
/// Unlike @c operator*, this is element-by-element multiplication, not a matrix product.
template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> matrixCompMult(const mat<N, T>& x, const mat<N, T>& y) noexcept;

/// @brief Returns the outer product of column vector @p c and row vector @p r.
///
/// The result is an N x N matrix where element @f$ [i][j] = c_i \cdot r_j @f$.
/// @param c Column vector.
/// @param r Row vector.
template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> outerProduct(const vec<N, T>& c, const vec<N, T>& r) noexcept;

/// @brief Returns the transpose of matrix @p m.
template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> transpose(const mat<N, T>& m) noexcept;

/// @brief Returns the determinant of square matrix @p m.
template <length_t N, typename T>
[[nodiscard]] constexpr T determinant(const mat<N, T>& m) noexcept;

/// @brief Returns the inverse of square matrix @p m.
///
/// The result is undefined if @p m is singular (determinant equals zero).
template <length_t N, typename T>
[[nodiscard]] constexpr mat<N, T> inverse(const mat<N, T>& m) noexcept;

}  // namespace mEn

#include "matrix_common.inl"
