#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> lessThan(const vec<L, T>& x, const vec<L, T>& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> lessThanEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> greaterThan(const vec<L, T>& x, const vec<L, T>& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> greaterThanEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> equal(const vec<L, T>& x, const vec<L, T>& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, bool> notEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept;

template <length_t L>
[[nodiscard]] constexpr bool any(const vec<L, bool>& v) noexcept;

template <length_t L>
[[nodiscard]] constexpr bool all(const vec<L, bool>& v) noexcept;

template <length_t L>
[[nodiscard]] constexpr vec<L, bool> not_(const vec<L, bool>& v) noexcept;

}  // namespace mEn

#include "vector_relational.inl"
