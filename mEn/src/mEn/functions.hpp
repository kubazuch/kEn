#pragma once

#include <cmath>

#include "mat3.hpp"
#include "mat4.hpp"
#include "quat.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace mEn {

template <typename T>
[[nodiscard]] constexpr T fmin(T a, T b);
template <typename T>
[[nodiscard]] constexpr T fmax(T a, T b);
template <typename T>
[[nodiscard]] constexpr T clamp(T val, T minVal, T maxVal);

template <typename T>
[[nodiscard]] const typename T::type* value_ptr(const T& value);
template <typename T>
[[nodiscard]] typename T::type value_ptr(T& value);

template <typename T>
[[nodiscard]] constexpr T pi();

template <typename T>
[[nodiscard]] constexpr T radians(T degrees) {
  return degrees * static_cast<T>(0.01745329251994329576923690768489);
}

using std::cos;
using std::sin;

}  // namespace mEn

#include "functions.inl"
#include "functions/mat_functions.hpp"
#include "functions/quat_functions.hpp"
#include "functions/vec_functions.hpp"