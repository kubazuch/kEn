#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

/// @brief Returns the absolute value of @p x, component-wise for vectors.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType abs(const GenType& x) noexcept;

/// @brief Returns the sign of @p x: 1 if positive, 0 if zero, -1 if negative, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType sign(const GenType& x) noexcept;

/// @brief Returns the largest integer value not greater than @p x, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType floor(const GenType& x) noexcept;

/// @brief Returns the nearest integer not greater in magnitude than @p x, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType trunc(const GenType& x) noexcept;

/// @brief Returns the nearest integer, rounding halfway cases away from zero, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType round(const GenType& x) noexcept;

/// @brief Returns the nearest even integer to @p x (banker's rounding), component-wise.
///
/// Halfway cases are rounded to the nearest even integer.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType roundEven(const GenType& x) noexcept;

/// @brief Returns the smallest integer value not less than @p x, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType ceil(const GenType& x) noexcept;

/// @brief Returns the fractional part of @p x, i.e. @f$ x - \lfloor x \rfloor @f$, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType fract(const GenType& x) noexcept;

/// @brief Returns the floating-point remainder of @f$ x / y @f$, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType mod(const GenType& x, const GenType& y) noexcept;

/// @brief Returns the floating-point remainder of @f$ x / y @f$ with a scalar divisor @p y.
template <length_t L, typename T>
[[nodiscard]] vec<L, T> mod(const vec<L, T>& x, T y) noexcept;

/// @brief Returns the component-wise minimum of @p x and @p y.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType min(const GenType& x, const GenType& y) noexcept;

/// @brief Returns the component-wise minimum of @p x and scalar @p y.
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, T> min(const vec<L, T>& x, T y) noexcept;

/// @brief Returns the component-wise maximum of @p x and @p y.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType max(const GenType& x, const GenType& y) noexcept;

/// @brief Returns the component-wise maximum of @p x and scalar @p y.
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, T> max(const vec<L, T>& x, T y) noexcept;

/// @brief Clamps @p x to the range [@p from, @p to], component-wise.
///
/// @pre @p from <= @p to
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] constexpr GenType clamp(const GenType& x, const GenType& from, const GenType& to) noexcept;

/// @brief Clamps each component of @p x to the scalar range [@p from, @p to].
///
/// @pre @p from <= @p to
template <length_t L, typename T>
[[nodiscard]] constexpr vec<L, T> clamp(const vec<L, T>& x, T from, T to) noexcept;

/// @brief Linear blend: @f$ x \cdot (1 - t) + y \cdot t @f$, component-wise.
///
/// When @p t is @c bool, returns @p x if @c false and @p y if @c true (component-wise select).
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
/// @tparam U Either @c bool or the scalar type of @p GenType.
template <VecOrScalar GenType, typename U>
  requires std::same_as<U, bool> || std::same_as<U, scalar_of_t<GenType>>
[[nodiscard]] constexpr GenType mix(const GenType& x, const GenType& y, U t) noexcept;

/// @brief Component-wise linear blend with a per-component blend factor vector @p t.
///
/// When @p t is @c vec<L, bool>, each component is selected from @p x (false) or @p y (true).
template <length_t L, typename T, typename U>
  requires std::same_as<U, bool> || std::same_as<U, T>
[[nodiscard]] constexpr vec<L, T> mix(const vec<L, T>& x, const vec<L, T>& y, const vec<L, U>& t) noexcept;

/// @brief Returns 0 if @p x < @p edge, otherwise 1, component-wise.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType step(const GenType& edge, const GenType& x) noexcept;

/// @brief Returns 0 if @p x < scalar @p edge, otherwise 1, component-wise.
template <length_t L, typename T>
[[nodiscard]] vec<L, T> step(T edge, const vec<L, T>& x) noexcept;

/// @brief Smooth Hermite interpolation between 0 and 1 for @p edge1 < @p x < @p edge2.
///
/// Returns 0 if @p x <= @p edge1 and 1 if @p x >= @p edge2.
/// Between the edges uses @f$ t^2(3 - 2t) @f$ where @f$ t = (x - edge1) / (edge2 - edge1) @f$.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] GenType smoothstep(const GenType& edge1, const GenType& edge2, const GenType& x) noexcept;

/// @brief Smooth Hermite interpolation with scalar edge values applied component-wise.
template <length_t L, typename T>
[[nodiscard]] vec<L, T> smoothstep(T edge1, T edge2, const vec<L, T>& x) noexcept;

/// @brief Returns @c true for each component that is NaN.
///
/// For a scalar @p GenType returns @c bool; for a vector type returns a @c bool_result_t vector.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] bool_result_t<GenType> isnan(const GenType& x) noexcept;

/// @brief Returns @c true for each component that is positive or negative infinity.
///
/// For a scalar @p GenType returns @c bool; for a vector type returns a @c bool_result_t vector.
/// @tparam GenType Scalar or vector type satisfying @c VecOrScalar.
template <VecOrScalar GenType>
[[nodiscard]] bool_result_t<GenType> isinf(const GenType& x) noexcept;

}  // namespace mEn

#include "common.inl"
