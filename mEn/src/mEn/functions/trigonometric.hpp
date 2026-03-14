#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType radians(const GenType& degrees) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType degrees(const GenType& radians) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType sin(const GenType& angle) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType cos(const GenType& angle) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType tan(const GenType& angle) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType asin(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType acos(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType atan(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType atan(const GenType& y, const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType sinh(const GenType& angle) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType cosh(const GenType& angle) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType tanh(const GenType& angle) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType asinh(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType acosh(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType atanh(const GenType& x) noexcept;

}  // namespace mEn

#include "trigonometric.inl"
