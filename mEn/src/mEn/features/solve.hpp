#pragma once

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Solves the linear system @f$ A x = b @f$ for @p x.
///
/// Uses Gaussian elimination with partial pivoting, which is more numerically
/// stable than forming @c inverse(A) and multiplying, and avoids the extra cost
/// of computing the full inverse when only a single right-hand side is needed.
///
/// On failure, @p x is left in an unspecified state.
///
/// @param a Coefficient matrix @f$ A @f$ (column-major, as everywhere in mEn).
/// @param b Right-hand side vector @f$ b @f$.
/// @param x Receives the solution vector @f$ x @f$.
/// @returns @c true on success, @c false if @p a is singular (a near-zero pivot
///          is encountered).
template <length_t N, typename T>
[[nodiscard]] bool solve(const mat<N, T>& a, const vec<N, T>& b, vec<N, T>& x) noexcept;

}  // namespace mEn

#include "solve.inl"
