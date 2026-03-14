#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <typename T>
[[nodiscard]] constexpr qua<T> identity() noexcept;

template <typename T>
[[nodiscard]] constexpr qua<T> rotate(const qua<T>& q, T angle, const vec<3, T>& v) noexcept;

template <typename T>
[[nodiscard]] constexpr qua<T> quatLookAt(const vec<3, T>& direction, const vec<3, T>& up) noexcept;

}  // namespace mEn

#include "quaternion_transform.inl"
