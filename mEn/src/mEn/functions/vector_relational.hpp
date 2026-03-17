#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

// TODO(kuzu): replace with operators and allow comparisons vec<L,T> < T

/// @brief Returns a bool vector where component @p i is @c true if @f$ x_i < y_i @f$.
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> lessThan(const vec<L, T>& x, const vec<L, T>& y) noexcept;

/// @brief Returns a bool vector where component @p i is @c true if @f$ x_i \leq y_i @f$.
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> lessThanEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept;

/// @brief Returns a bool vector where component @p i is @c true if @f$ x_i > y_i @f$.
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> greaterThan(const vec<L, T>& x, const vec<L, T>& y) noexcept;

/// @brief Returns a bool vector where component @p i is @c true if @f$ x_i \geq y_i @f$.
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> greaterThanEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept;

/// @brief Returns a bool vector where component @p i is @c true if @f$ x_i = y_i @f$.
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> equal(const vec<L, T>& x, const vec<L, T>& y) noexcept;

/// @brief Returns a bool vector where component @p i is @c true if @f$ x_i \neq y_i @f$.
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> notEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept;

/// @brief Returns @c true if any component of @p v is @c true.
template <length_t L>
[[nodiscard]] constexpr bool any(const vec<L, bool>& v) noexcept;

/// @brief Returns @c true if all components of @p v are @c true.
template <length_t L>
[[nodiscard]] constexpr bool all(const vec<L, bool>& v) noexcept;

/// @brief Returns the component-wise logical NOT of bool vector @p v.
///
/// Named @c not_ with a trailing underscore to avoid conflict with the C++ keyword @c not.
template <length_t L>
[[nodiscard]] constexpr vec<L, bool> not_(const vec<L, bool>& v) noexcept;  // NOLINT(readability-identifier-naming)

}  // namespace mEn

#include "vector_relational.inl"
