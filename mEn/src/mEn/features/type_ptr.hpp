#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <typename GenType>
[[nodiscard]] constexpr const typename GenType::value_t* value_ptr(const GenType& v) noexcept;

template <typename GenType>
[[nodiscard]] constexpr typename GenType::value_t* value_ptr(GenType& v) noexcept;

template <typename T>
[[nodiscard]] constexpr vec<2, T> make_vec2(const T* ptr) noexcept;

template <typename T>
[[nodiscard]] constexpr vec<3, T> make_vec3(const T* ptr) noexcept;

template <typename T>
[[nodiscard]] constexpr vec<4, T> make_vec4(const T* ptr) noexcept;

template <typename T>
[[nodiscard]] constexpr mat<3, T> make_mat3(const T* ptr) noexcept;

template <typename T>
[[nodiscard]] constexpr mat<4, T> make_mat4(const T* ptr) noexcept;

template <typename T>
[[nodiscard]] constexpr qua<T> make_quat(const T* ptr) noexcept;

}  // namespace mEn

#include "type_ptr.inl"
