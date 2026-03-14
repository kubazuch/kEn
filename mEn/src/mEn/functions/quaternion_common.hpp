#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <typename T>
[[nodiscard]] qua<T> mix(const qua<T>& x, const qua<T>& y, T t) noexcept;

template <typename T>
[[nodiscard]] constexpr qua<T> lerp(const qua<T>& x, const qua<T>& y, T t) noexcept;

template <typename T>
[[nodiscard]] qua<T> slerp(const qua<T>& x, const qua<T>& y, T t) noexcept;

template <typename T>
[[nodiscard]] constexpr qua<T> conjugate(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] constexpr qua<T> inverse(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] vec<4, bool> isnan(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] vec<4, bool> isinf(const qua<T>& q) noexcept;

}  // namespace mEn

#include "quaternion_common.inl"
