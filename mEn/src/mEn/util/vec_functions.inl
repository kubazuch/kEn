#pragma once

#include <cmath>

#include "vec_functions.hpp"

namespace mEn {

template <int N, typename T>
inline T length(const vec<N, T>& vec) {
  return std::sqrt(dot(vec, vec));
}

template <int N, typename T>
inline T distance(const vec<N, T>& p0, const vec<N, T>& p1) {
  return length(p1 - p0);
}

template <typename T>
inline constexpr T dot(const vec<2, T>& u, const vec<2, T>& v) {
  vec<2, T> tmp(u * v);
  return tmp.x + tmp.y;
}

template <typename T>
inline constexpr T dot(const vec<3, T>& u, const vec<3, T>& v) {
  vec<3, T> tmp(u * v);
  return tmp.x + tmp.y + tmp.z;
}

template <typename T>
inline constexpr T dot(const vec<4, T>& u, const vec<4, T>& v) {
  vec<4, T> tmp(u * v);
  return tmp.x + tmp.y + tmp.z + tmp.w;
}

template <typename T>
inline constexpr vec<3, T> cross(const vec<3, T>& u, const vec<3, T>& v) {
  return vec<3, T>(u.y * v.z - v.y * u.z, u.z * v.x - v.z * u.x, u.x * v.y - v.x * u.y);
}

template <int N, typename T>
inline vec<N, T> normalize(const vec<N, T>& vec) {
  return vec * static_cast<T>(1) / std::sqrt(dot(vec, vec));
}

}  // namespace mEn