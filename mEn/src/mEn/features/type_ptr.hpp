#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

/// @brief Returns a const pointer to the first component of @p v in memory.
///
/// The pointer addresses contiguous storage: components are laid out in the
/// natural field order of the type (x, y, z, w for vectors and quaternions;
/// column-major for matrices).  Intended for passing mEn types directly to
/// graphics APIs (e.g. @c glUniform*, @c glBufferData).
///
/// @tparam GenType A vector, matrix, or quaternion type exposing @c value_t.
/// @param v  The object whose raw data is requested.
/// @returns Const pointer to the first component.
template <typename GenType>
[[nodiscard]] constexpr const GenType::value_t* value_ptr(const GenType& v) noexcept;

/// @brief Returns a mutable pointer to the first component of @p v in memory.
///
/// Writes through the returned pointer are immediately visible via @p v.
/// See the const overload for layout details.
///
/// @tparam GenType A vector, matrix, or quaternion type exposing @c value_t.
/// @param v  The object whose raw data is requested.
/// @returns Mutable pointer to the first component.
template <typename GenType>
[[nodiscard]] constexpr GenType::value_t* value_ptr(GenType& v) noexcept;

/// @brief Constructs a @c vec2 from the first two scalars at @p ptr.
/// @param ptr Pointer to at least 2 contiguous scalars in @c {x, y} order.
template <typename T>
[[nodiscard]] constexpr vec<2, T> make_vec2(const T* ptr) noexcept;

/// @brief Constructs a @c vec3 from the first three scalars at @p ptr.
/// @param ptr Pointer to at least 3 contiguous scalars in @c {x, y, z} order.
template <typename T>
[[nodiscard]] constexpr vec<3, T> make_vec3(const T* ptr) noexcept;

/// @brief Constructs a @c vec4 from the first four scalars at @p ptr.
/// @param ptr Pointer to at least 4 contiguous scalars in @c {x, y, z, w} order.
template <typename T>
[[nodiscard]] constexpr vec<4, T> make_vec4(const T* ptr) noexcept;

/// @brief Constructs a @c mat3 from nine contiguous scalars at @p ptr.
/// @param ptr Pointer to at least 9 scalars in column-major order
///            @c {col0.x, col0.y, col0.z, col1.x, ...}.
template <typename T>
[[nodiscard]] constexpr mat<3, T> make_mat3(const T* ptr) noexcept;

/// @brief Constructs a @c mat4 from sixteen contiguous scalars at @p ptr.
/// @param ptr Pointer to at least 16 scalars in column-major order
///            @c {col0.x, col0.y, col0.z, col0.w, col1.x, ...}.
template <typename T>
[[nodiscard]] constexpr mat<4, T> make_mat4(const T* ptr) noexcept;

/// @brief Constructs a quaternion from four contiguous scalars at @p ptr.
/// @param ptr Pointer to at least 4 scalars in @c {x, y, z, w} order.
template <typename T>
[[nodiscard]] constexpr qua<T> make_quat(const T* ptr) noexcept;

}  // namespace mEn

#include "type_ptr.inl"
