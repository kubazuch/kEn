#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <length_t L, typename T>
[[nodiscard]] T length(const vec<L, T>& x) noexcept;

template <length_t L, typename T>
[[nodiscard]] T distance(const vec<L, T>& x, const vec<L, T>& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr T dot(const vec<L, T>& x, const vec<L, T>& y) noexcept;

template <typename T>
[[nodiscard]] constexpr vec<3, T> cross(const vec<3, T>& x, const vec<3, T>& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] vec<L, T> normalize(const vec<L, T>& x) noexcept;

template <length_t L, typename T>
[[nodiscard]] vec<L, T> faceforward(const vec<L, T>& N, const vec<L, T>& I, const vec<L, T>& Nref) noexcept;

template <length_t L, typename T>
[[nodiscard]] vec<L, T> reflect(const vec<L, T>& I, const vec<L, T>& N) noexcept;

template <length_t L, typename T>
[[nodiscard]] vec<L, T> refract(const vec<L, T>& I, const vec<L, T>& N, T eta) noexcept;

}  // namespace mEn

#include "geometric.inl"
