#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <typename T>
[[nodiscard]] qua<T> exp(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] qua<T> log(const qua<T>& q) noexcept;

template <typename T>
[[nodiscard]] qua<T> pow(const qua<T>& q, T x) noexcept;

template <typename T>
[[nodiscard]] qua<T> sqrt(const qua<T>& q) noexcept;

}  // namespace mEn

#include "quaternion_exponential.inl"
