#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <typename T>
[[nodiscard]] T length(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] qua<T> normalize(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] constexpr T dot(const qua<T>& x, const qua<T>& y) noexcept;

template <typename T>
[[nodiscard]] constexpr qua<T> cross(const qua<T>& x, const qua<T>& y) noexcept;

}  // namespace mEn

#include "quaternion_geometric.inl"
