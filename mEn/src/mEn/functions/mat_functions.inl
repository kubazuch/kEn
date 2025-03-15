#pragma once

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

}  // namespace mEn