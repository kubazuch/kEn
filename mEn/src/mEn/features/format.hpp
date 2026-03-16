#pragma once

#include <format>

#include <mEn/fwd.hpp>

// NOLINTBEGIN(bugprone-std-namespace-modification)
namespace std {

/// @brief @c std::formatter specialization for @c mEn::vec<L,T>.
/// Produces @c (x, y[, z[, w]]).
template <mEn::length_t L, typename T>
struct formatter<mEn::vec<L, T>>;

/// @brief @c std::formatter specialization for @c mEn::mat<N,T>.
/// Produces @c ((row0), (row1), ...) in logical row-major order.
template <mEn::length_t N, typename T>
struct formatter<mEn::mat<N, T>>;

/// @brief @c std::formatter specialization for @c mEn::qua<T>.
/// Produces @c (w, x, y, z).
template <typename T>
struct formatter<mEn::qua<T>>;

}  // namespace std
// NOLINTEND(bugprone-std-namespace-modification)

#include "format.inl"
