#pragma once

#include <mEn/fwd.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Returns the Euclidean length (magnitude) of vector @p x: @f$ \|x\| @f$.
template <length_t L, typename T>
[[nodiscard]] T length(const vec<L, T>& x) noexcept;

/// @brief Returns the Euclidean distance between vectors @p x and @p y: @f$ \|y - x\| @f$.
template <length_t L, typename T>
[[nodiscard]] T distance(const vec<L, T>& x, const vec<L, T>& y) noexcept;

/// @brief Returns the dot product @f$ x \cdot y = \sum_i x_i y_i @f$.
template <length_t L, typename T>
[[nodiscard]] constexpr T dot(const vec<L, T>& x, const vec<L, T>& y) noexcept;

/// @brief Returns the cross product @f$ x \times y @f$ of two 3-component vectors.
template <typename T>
[[nodiscard]] constexpr vec<3, T> cross(const vec<3, T>& x, const vec<3, T>& y) noexcept;

/// @brief Returns a unit-length vector in the direction of @p x.
///
/// If @p x has zero length the result is undefined.
template <length_t L, typename T>
[[nodiscard]] vec<L, T> normalize(const vec<L, T>& x) noexcept;

/// @brief Returns @p N if @f$ I \cdot Nref < 0 @f$, otherwise returns @f$ -N @f$.
///
/// Used to orient a surface normal to face the same side as the incoming ray.
/// @param N    Surface normal to orient.
/// @param I    Incident vector.
/// @param Nref Reference normal used to determine orientation.
template <length_t L, typename T>
[[nodiscard]] vec<L, T> faceforward(const vec<L, T>& N, const vec<L, T>& I, const vec<L, T>& Nref) noexcept;

/// @brief Returns the reflection of incident vector @p I about surface normal @p N.
///
/// Computes @f$ I - 2 \cdot (N \cdot I) \cdot N @f$.
/// @pre @p N must be unit-length.
/// @param I Incident (incoming) vector.
/// @param N Unit surface normal.
template <length_t L, typename T>
[[nodiscard]] vec<L, T> reflect(const vec<L, T>& I, const vec<L, T>& N) noexcept;

/// @brief Returns the refraction of incident vector @p I through a surface with normal @p N.
///
/// Computes the refracted direction using Snell's law.
/// Returns a zero vector when total internal reflection occurs (@f$ \eta^2(1 - (N \cdot I)^2) > 1 @f$).
/// @pre @p I and @p N must be unit-length.
/// @param I   Unit incident vector.
/// @param N   Unit surface normal pointing away from the surface.
/// @param eta Ratio of the index of refraction (incident medium / transmitted medium).
template <length_t L, typename T>
[[nodiscard]] vec<L, T> refract(const vec<L, T>& I, const vec<L, T>& N, T eta) noexcept;

}  // namespace mEn

#include "geometric.inl"
