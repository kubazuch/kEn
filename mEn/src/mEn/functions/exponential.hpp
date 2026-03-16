#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

/// @brief Raises @p base to the power @p exponent, component-wise.
///
/// @pre @p base >= 0, or the result is undefined.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType pow(const GenType& base, const GenType& exponent) noexcept;

/// @brief Returns the natural exponent @f$ e^x @f$, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType exp(const GenType& x) noexcept;

/// @brief Returns the natural logarithm @f$ \ln(x) @f$, component-wise.
///
/// @pre @p x > 0
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType log(const GenType& x) noexcept;

/// @brief Returns @f$ 2^x @f$, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType exp2(const GenType& x) noexcept;

/// @brief Returns the base-2 logarithm @f$ \log_2(x) @f$, component-wise.
///
/// @pre @p x > 0
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType log2(const GenType& x) noexcept;

/// @brief Returns the base-10 logarithm @f$ \log_{10}(x) @f$, component-wise.
///
/// @pre @p x > 0
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType log10(const GenType& x) noexcept;

/// @brief Returns the square root @f$ \sqrt{x} @f$, component-wise.
///
/// @pre @p x >= 0
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType sqrt(const GenType& x) noexcept;

/// @brief Returns the inverse square root @f$ 1 / \sqrt{x} @f$, component-wise.
///
/// @pre @p x > 0
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType inversesqrt(const GenType& x) noexcept;

}  // namespace mEn

#include "exponential.inl"
