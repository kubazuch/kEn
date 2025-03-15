#pragma once
#include <utility>

#include "common.hpp"
#include "matrix.hpp"

namespace mEn {

namespace impl {

template <int N, typename T, std::size_t... I>
constexpr mat<N, T> matrixCompMult(const mat<N, T>& x, const mat<N, T>& y, std::index_sequence<I...>) {
  return mat<N, T>{(x[I] * y[I])...};
}

template <int N, typename T, std::size_t... I>
constexpr mat<N, T> outerProduct(const vec<N, T>& c, const vec<N, T>& r, std::index_sequence<I...>) {
  return mat<N, T>{(c * r[I])...};
}

}  // namespace impl

template <int N, typename T>
inline constexpr mat<N, T> matrixCompMult(const mat<N, T>& x, const mat<N, T>& y) {
  return impl::matrixCompMult(x, y, std::make_index_sequence<N>{});
}

template <int N, typename T>
inline constexpr mat<N, T> outerProduct(const vec<N, T>& c, const vec<N, T>& r) {
  return impl::outerProduct(c, r, std::make_index_sequence<N>{});
}

template <typename T>
inline constexpr mat<3, T> transpose(const mat<3, T>& m) {
  mat<3, T> res(1);

  res[0][0] = m[0][0];
  res[0][1] = m[1][0];
  res[0][2] = m[2][0];

  res[1][0] = m[0][1];
  res[1][1] = m[1][1];
  res[1][2] = m[2][1];

  res[2][0] = m[0][2];
  res[2][1] = m[1][2];
  res[2][2] = m[2][2];

  return res;
}

template <typename T>
inline constexpr mat<4, T> transpose(const mat<4, T>& m) {
  mat<4, T> res(1);

  res[0][0] = m[0][0];
  res[0][1] = m[1][0];
  res[0][2] = m[2][0];
  res[0][3] = m[3][0];

  res[1][0] = m[0][1];
  res[1][1] = m[1][1];
  res[1][2] = m[2][1];
  res[1][3] = m[3][1];

  res[2][0] = m[0][2];
  res[2][1] = m[1][2];
  res[2][2] = m[2][2];
  res[2][3] = m[3][2];

  res[3][0] = m[0][3];
  res[3][1] = m[1][3];
  res[3][2] = m[2][3];
  res[3][3] = m[3][3];
  return res;
}

template <typename T>
inline constexpr T determinant(const mat<3, T>& m) {
  vec<3, T> cofactors{+(m[1][1] * m[2][2] - m[2][1] * m[1][2]),  //
                      -(m[1][0] * m[2][2] - m[2][0] * m[1][2]),  //
                      +(m[1][0] * m[2][1] - m[2][0] * m[1][1])};

  return m[0][0] * cofactors[0] + m[0][1] * cofactors[1] + m[0][2] * cofactors[2];
}

template <typename T>
inline constexpr T determinant(const mat<4, T>& m) {
  T minor01 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
  T minor02 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
  T minor03 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
  T minor12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
  T minor13 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
  T minor23 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

  vec<4, T> cofactors{+(m[1][1] * minor01 - m[1][2] * minor02 + m[1][3] * minor03),  //
                      -(m[1][0] * minor01 - m[1][2] * minor12 + m[1][3] * minor13),  //
                      +(m[1][0] * minor02 - m[1][1] * minor12 + m[1][3] * minor23),  //
                      -(m[1][0] * minor03 - m[1][1] * minor13 + m[1][2] * minor23)};

  return m[0][0] * cofactors[0] + m[0][1] * cofactors[1] + m[0][2] * cofactors[2] + m[0][3] * cofactors[3];
}

template <typename T>
inline constexpr mat<3, T> inverse(const mat<3, T>& m) {
  T minor_12_01 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
  T minor_12_02 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
  T minor_12_12 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

  T det     = m[0][0] * minor_12_12 - m[0][1] * minor_12_02 + m[0][2] * minor_12_01;
  T inv_det = 1.0F / det;

  mat<3, T> res(1);
  res[0][0] = +minor_12_12 * inv_det;
  res[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * inv_det;
  res[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * inv_det;

  res[1][0] = -minor_12_02 * inv_det;
  res[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * inv_det;
  res[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * inv_det;

  res[2][0] = +minor_12_01 * inv_det;
  res[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * inv_det;
  res[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * inv_det;

  return res;
}

template <typename T>
inline constexpr mat<4, T> inverse(const mat<4, T>& m) {
  // Calculate minors
  T minor_01_01 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
  T minor_02_01 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
  T minor_03_01 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

  T minor_01_02 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
  T minor_02_02 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
  T minor_03_02 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

  T minor_01_03 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
  T minor_02_03 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
  T minor_03_03 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

  T minor_01_12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
  T minor_02_12 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
  T minor_03_12 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

  T minor_01_13 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
  T minor_02_13 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
  T minor_03_13 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

  T minor_01_23 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
  T minor_02_23 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
  T minor_03_23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

  // Prepare adjugate
  // Cofactors are calculated by laplace expansion on first column, except when it's not present
  vec<4, T> laplace0(m[1][0], m[0][0], m[0][0], m[0][0]);
  vec<4, T> laplace1(m[1][1], m[0][1], m[0][1], m[0][1]);
  vec<4, T> laplace2(m[1][2], m[0][2], m[0][2], m[0][2]);
  vec<4, T> laplace3(m[1][3], m[0][3], m[0][3], m[0][3]);

  vec<4, T> minors_01(minor_01_01, minor_01_01, minor_02_01, minor_03_01);
  vec<4, T> minors_02(minor_01_02, minor_01_02, minor_02_02, minor_03_02);
  vec<4, T> minors_03(minor_01_03, minor_01_03, minor_02_03, minor_03_03);
  vec<4, T> minors_12(minor_01_12, minor_01_12, minor_02_12, minor_03_12);
  vec<4, T> minors_13(minor_01_13, minor_01_13, minor_02_13, minor_03_13);
  vec<4, T> minors_23(minor_01_23, minor_01_23, minor_02_23, minor_03_23);

  vec<4, T> signs(1, -1, 1, -1);
  mat<4, T> adjugate(+signs * (laplace1 * minors_01 - laplace2 * minors_02 + laplace3 * minors_03),  //
                     -signs * (laplace0 * minors_01 - laplace2 * minors_12 + laplace3 * minors_13),  //
                     +signs * (laplace0 * minors_02 - laplace1 * minors_12 + laplace3 * minors_23),  //
                     -signs * (laplace0 * minors_03 - laplace1 * minors_13 + laplace2 * minors_23));

  T det     = m[0][0] * adjugate[0][0] + m[0][1] * adjugate[1][0] + m[0][2] * adjugate[2][0] + m[0][3] * adjugate[3][0];
  T inv_det = 1.0F / det;

  return adjugate * inv_det;
}

}  // namespace mEn