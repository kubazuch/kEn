#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <VecOrScalar GenType>
[[nodiscard]] GenType pow(const GenType& base, const GenType& exponent) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType exp(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType log(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType exp2(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType log2(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType log10(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType sqrt(const GenType& x) noexcept;

template <VecOrScalar GenType>
[[nodiscard]] GenType inversesqrt(const GenType& x) noexcept;

}  // namespace mEn

#include "exponential.inl"
