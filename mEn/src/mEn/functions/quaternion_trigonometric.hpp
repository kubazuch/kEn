#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <typename T>
[[nodiscard]] T angle(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] vec<3, T> axis(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] qua<T> angleAxis(T angle, const vec<3, T>& axis) noexcept;

template <typename T>
[[nodiscard]] vec<3, T> eulerAngles(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] T roll(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] T pitch(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] T yaw(const qua<T>& q) noexcept;

}  // namespace mEn

#include "quaternion_trigonometric.inl"
