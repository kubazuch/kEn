#pragma once

#include <cmath>

#include "mat_functions.hpp"

namespace mEn {

template <int N, typename T>
inline constexpr mat<N, T> identity() {
  return mat<N, T>(1);
}

template <typename T>
inline constexpr mat<4, T> translate(const mat<4, T>& m, const vec<3, T>& v) {
  mat<4, T> ret(m);
  ret[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
  return ret;
}

template <typename T>
inline mat<4, T> scale(const mat<4, T>& m, const vec<3, T>& v) {
  mat<4, T> ret;
  ret[0] = m[0] * v[0];
  ret[1] = m[1] * v[1];
  ret[2] = m[2] * v[2];
  ret[3] = m[3];
  return ret;
}

template <typename T>
inline mat<4, T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) {
  mat<4, T> ret(1);
  ret[0][0] = static_cast<T>(2) / (right - left);
  ret[1][1] = static_cast<T>(2) / (top - bottom);
  ret[2][2] = -static_cast<T>(2) / (zFar - zNear);
  ret[3][0] = -(right + left) / (right - left);
  ret[3][1] = -(top + bottom) / (top - bottom);
  ret[3][2] = -(zFar + zNear) / (zFar - zNear);

  return ret;
}

template <typename T>
inline mat<4, T> perspective(T fov, T aspect, T zNear, T zFar) {
  const T tan_half_fov = std::tan(fov / static_cast<T>(2));

  mat<4, T> ret(0);
  ret[0][0] = static_cast<T>(1) / (tan_half_fov * aspect);
  ret[1][1] = static_cast<T>(1) / (tan_half_fov);
  ret[2][2] = -(zFar + zNear) / (zFar - zNear);
  ret[2][3] = -static_cast<T>(1);
  ret[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

  return ret;
}

}  // namespace mEn