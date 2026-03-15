#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

/// @brief Converts degrees to radians: @f$ d \cdot \pi / 180 @f$, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType radians(const GenType& degrees) noexcept;

/// @brief Converts radians to degrees: @f$ r \cdot 180 / \pi @f$, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType degrees(const GenType& radians) noexcept;

/// @brief Returns the sine of @p angle (in radians), component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType sin(const GenType& angle) noexcept;

/// @brief Returns the cosine of @p angle (in radians), component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType cos(const GenType& angle) noexcept;

/// @brief Returns the tangent of @p angle (in radians), component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType tan(const GenType& angle) noexcept;

/// @brief Returns the arc sine in the range @f$ [-\pi/2, \pi/2] @f$, component-wise.
///
/// @pre Each component of @p x must be in @f$ [-1, 1] @f$.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType asin(const GenType& x) noexcept;

/// @brief Returns the arc cosine in the range @f$ [0, \pi] @f$, component-wise.
///
/// @pre Each component of @p x must be in @f$ [-1, 1] @f$.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType acos(const GenType& x) noexcept;

/// @brief Returns the arc tangent of @p x in the range @f$ [-\pi/2, \pi/2] @f$, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType atan(const GenType& x) noexcept;

/// @brief Returns the arc tangent of @f$ y / x @f$ using both signs to determine the quadrant.
///
/// Two-argument atan (equivalent to @c std::atan2). Result is in @f$ (-\pi, \pi] @f$.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType atan(const GenType& y, const GenType& x) noexcept;

/// @brief Returns the hyperbolic sine of @p angle, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType sinh(const GenType& angle) noexcept;

/// @brief Returns the hyperbolic cosine of @p angle, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType cosh(const GenType& angle) noexcept;

/// @brief Returns the hyperbolic tangent of @p angle, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType tanh(const GenType& angle) noexcept;

/// @brief Returns the inverse hyperbolic sine of @p x, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType asinh(const GenType& x) noexcept;

/// @brief Returns the inverse hyperbolic cosine of @p x, component-wise.
///
/// @pre Each component of @p x must be >= 1.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType acosh(const GenType& x) noexcept;

/// @brief Returns the inverse hyperbolic tangent of @p x, component-wise.
///
/// @pre Each component of @p x must be in @f$ (-1, 1) @f$.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType atanh(const GenType& x) noexcept;

}  // namespace mEn

#include "trigonometric.inl"
