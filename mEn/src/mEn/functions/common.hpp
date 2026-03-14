#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType abs(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType sign(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType floor(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType trunc(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType round(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType roundEven(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType ceil(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType fract(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType mod(const GenType& x, const GenType& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] vec<L, T> mod(const vec<L, T>& x, T y) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType min(const GenType& x, const GenType& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, T> min(const vec<L, T>& x, T y) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType max(const GenType& x, const GenType& y) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, T> max(const vec<L, T>& x, T y) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType clamp(const GenType& x, const GenType& from, const GenType& to) noexcept;

template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, T> clamp(const vec<L, T>& x, T from, T to) noexcept;

template <VecOrScalar GenType, typename U>
  requires std::same_as<U, bool> || std::same_as<U, scalar_of_t<GenType>>
[[nodiscard]] constexpr GenType mix(const GenType& x, const GenType& y, U t) noexcept;

template <length_t L, typename T, typename U>
  requires std::same_as<U, bool> || std::same_as<U, T>
[[nodiscard]] constexpr vec<L, T> mix(const vec<L, T>& x, const vec<L, T>& y, const vec<L, U>& t) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType step(const GenType& edge, const GenType& x) noexcept;

template <length_t L, typename T>
[[nodiscard]] vec<L, T> step(T edge, const vec<L, T>& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType smoothstep(const GenType& edge1, const GenType& edge2, const GenType& x) noexcept;

template <length_t L, typename T>
[[nodiscard]] vec<L, T> smoothstep(T edge1, T edge2, const vec<L, T>& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] bool_result_t<GenType> isnan(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] bool_result_t<GenType> isinf(const GenType& x) noexcept;

}  // namespace mEn

#include "common.inl"
