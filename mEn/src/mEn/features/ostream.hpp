#pragma once

#include <iosfwd>

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Writes a vec<L, T> to an output stream as @c (x, y[, z[, w]]).
template <length_t L, typename T>
std::ostream& operator<<(std::ostream& os, const vec<L, T>& v);

/// @brief Writes a mat<N, T> to an output stream as @c ((row0), (row1), ...).
///
/// Rows are presented in logical (row-major) order even though storage is column-major.
template <length_t N, typename T>
std::ostream& operator<<(std::ostream& os, const mat<N, T>& m);

/// @brief Writes a qua<T> to an output stream as @c (x, y, z, w).
template <typename T>
std::ostream& operator<<(std::ostream& os, const qua<T>& q);

}  // namespace mEn

#include "ostream.inl"
