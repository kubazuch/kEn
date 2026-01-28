#pragma once
#include "matrix.hpp"

namespace mEn {

// Explicit constructors
template <typename T>
inline constexpr mat<4, T>::mat(T scalar)
    : value_{col_t(scalar, 0, 0, 0), col_t(0, scalar, 0, 0), col_t(0, 0, scalar, 0), col_t(0, 0, 0, scalar)} {}
template <typename T>
inline constexpr mat<4, T>::mat(T x0, T y0, T z0, T w0,  //
                                T x1, T y1, T z1, T w1,  //
                                T x2, T y2, T z2, T w2,  //
                                T x3, T y3, T z3, T w3)
    : value_{col_t(x0, y0, z0, w0), col_t(x1, y1, z1, w1), col_t(x2, y2, z2, w2), col_t(x3, y3, z3, w3)} {}
template <typename T>
inline constexpr mat<4, T>::mat(const col_t& v0, const col_t& v1, const col_t& v2, const col_t& v3)
    : value_{col_t(v0), col_t(v1), col_t(v2), col_t(v3)} {}

// Conversion constructors
template <typename T>
template <typename X0, typename Y0, typename Z0, typename W0,  //
          typename X1, typename Y1, typename Z1, typename W1,  //
          typename X2, typename Y2, typename Z2, typename W2,  //
          typename X3, typename Y3, typename Z3, typename W3>
inline constexpr mat<4, T>::mat(X0 x0, Y0 y0, Z0 z0, W0 w0,  //
                                X1 x1, Y1 y1, Z1 z1, W1 w1,  //
                                X2 x2, Y2 y2, Z2 z2, W2 w2,  //
                                X3 x3, Y3 y3, Z3 z3, W3 w3)
    : value_{col_t(x0, y0, z0, w0), col_t(x1, y1, z1, w1), col_t(x2, y2, z2, w2), col_t(x3, y3, z3, w3)} {}

template <typename T>
template <typename U>
inline constexpr mat<4, T>::mat(const mat<3, U>& m)
    : value_{col_t(m[0], 0), col_t(m[1], 0), col_t(m[2], 0), col_t(0, 0, 0, 1)} {};

// Components
template <typename T>
inline constexpr mat<4, T>::col_t& mat<4, T>::operator[](int i) noexcept {
  assert(i >= 0 && i < length());
  return this->value_[i];
}

template <typename T>
inline constexpr const mat<4, T>::col_t& mat<4, T>::operator[](int i) const noexcept {
  assert(i >= 0 && i < length());
  return this->value_[i];
}

// Unary arithmetic operators
template <typename T>
template <typename U>
inline constexpr mat<4, T>& mat<4, T>::operator+=(U scalar) {
  this->value_[0] += scalar;
  this->value_[1] += scalar;
  this->value_[2] += scalar;
  this->value_[3] += scalar;
  return *this;
}

template <typename T>
template <typename U>
inline constexpr mat<4, T>& mat<4, T>::operator+=(const mat<4, U>& m) {
  this->value_[0] += m[0];
  this->value_[1] += m[1];
  this->value_[2] += m[2];
  this->value_[3] += m[3];
  return *this;
}

template <typename T>
template <typename U>
inline constexpr mat<4, T>& mat<4, T>::operator-=(U scalar) {
  this->value_[0] -= scalar;
  this->value_[1] -= scalar;
  this->value_[2] -= scalar;
  this->value_[3] -= scalar;
  return *this;
}

template <typename T>
template <typename U>
inline constexpr mat<4, T>& mat<4, T>::operator-=(const mat<4, U>& m) {
  this->value_[0] -= m[0];
  this->value_[1] -= m[1];
  this->value_[2] -= m[2];
  this->value_[3] -= m[3];
  return *this;
}

template <typename T>
template <typename U>
inline constexpr mat<4, T>& mat<4, T>::operator*=(U scalar) {
  this->value_[0] *= scalar;
  this->value_[1] *= scalar;
  this->value_[2] *= scalar;
  this->value_[3] *= scalar;
  return *this;
}

template <typename T>
template <typename U>
inline constexpr mat<4, T>& mat<4, T>::operator*=(const mat<4, U>& m) {
  return (*this = *this * m);
}

template <typename T>
template <typename U>
inline constexpr mat<4, T>& mat<4, T>::operator/=(U scalar) {
  this->value_[0] /= scalar;
  this->value_[1] /= scalar;
  this->value_[2] /= scalar;
  this->value_[3] /= scalar;
  return *this;
}

template <typename T>
template <typename U>
inline constexpr mat<4, T>& mat<4, T>::operator/=(const mat<4, U>& m) {
  return *this *= inverse(m);
}

// Increment and decrement operators
template <typename T>
inline constexpr mat<4, T>& mat<4, T>::operator++() {
  ++this->value_[0];
  ++this->value_[1];
  ++this->value_[2];
  ++this->value_[3];
  return *this;
}

template <typename T>
inline constexpr mat<4, T>& mat<4, T>::operator--() {
  --this->value_[0];
  --this->value_[1];
  --this->value_[2];
  --this->value_[3];
  return *this;
}

template <typename T>
inline constexpr mat<4, T> mat<4, T>::operator++(int) {
  mat<4, T> result(*this);
  ++*this;
  return result;
}

template <typename T>
inline constexpr mat<4, T> mat<4, T>::operator--(int) {
  mat<4, T> result(*this);
  --*this;
  return result;
}

// Unary operators
template <typename T>
inline constexpr mat<4, T> operator+(const mat<4, T>& m) {
  return m;
}

template <typename T>
inline constexpr mat<4, T> operator-(const mat<4, T>& m) {
  return mat<4, T>(-m[0], -m[1], -m[2], -m[3]);
}

// Binary arithmetic operatorstemplate <typename T>
template <typename T>
inline constexpr mat<4, T> operator+(const mat<4, T>& m, T scalar) {
  return mat<4, T>(m[0] + scalar, m[1] + scalar, m[2] + scalar, m[3] + scalar);
}

template <typename T>
inline constexpr mat<4, T> operator+(T scalar, const mat<4, T>& m) {
  return mat<4, T>(scalar + m[0], scalar + m[1], scalar + m[2], scalar + m[3]);
}

template <typename T>
inline constexpr mat<4, T> operator+(const mat<4, T>& m1, const mat<4, T>& m2) {
  return mat<4, T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3]);
}

template <typename T>
inline constexpr mat<4, T> operator-(const mat<4, T>& m, T scalar) {
  return mat<4, T>(m[0] - scalar, m[1] - scalar, m[2] - scalar, m[3] - scalar);
}

template <typename T>
inline constexpr mat<4, T> operator-(T scalar, const mat<4, T>& m) {
  return mat<4, T>(scalar - m[0], scalar - m[1], scalar - m[2], scalar - m[3]);
}

template <typename T>
inline constexpr mat<4, T> operator-(const mat<4, T>& m1, const mat<4, T>& m2) {
  return mat<4, T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2], m1[3] - m2[3]);
}

template <typename T>
inline constexpr mat<4, T> operator*(const mat<4, T>& m, T scalar) {
  return mat<4, T>(m[0] * scalar, m[1] * scalar, m[2] * scalar, m[3] * scalar);
}

template <typename T>
inline constexpr mat<4, T> operator*(T scalar, const mat<4, T>& m) {
  return mat<4, T>(scalar * m[0], scalar * m[1], scalar * m[2], scalar * m[3]);
}

template <typename T>
inline constexpr mat<4, T>::col_t operator*(const mat<4, T>& m, const typename mat<4, T>::row_t& v) {
  return typename mat<4, T>::col_t(m[0].x * v.x + m[1].x * v.y + m[2].x * v.z + m[3].x * v.w,
                                   m[0].y * v.x + m[1].y * v.y + m[2].y * v.z + m[3].y * v.w,
                                   m[0].z * v.x + m[1].z * v.y + m[2].z * v.z + m[3].z * v.w,
                                   m[0].w * v.x + m[1].w * v.y + m[2].w * v.z + m[3].w * v.w);
}

template <typename T>
inline constexpr mat<4, T>::row_t operator*(const typename mat<4, T>::col_t& v, const mat<4, T>& m) {
  return typename mat<4, T>::row_t(v.x * m[0].x + v.y * m[0].y + v.z * m[0].z + v.w * m[0].w,
                                   v.x * m[1].x + v.y * m[1].y + v.z * m[1].z + v.w * m[1].w,
                                   v.x * m[2].x + v.y * m[2].y + v.z * m[2].z + v.w * m[2].w,
                                   v.x * m[3].x + v.y * m[3].y + v.z * m[3].z + v.w * m[3].w);
}

template <typename T>
inline constexpr mat<4, T> operator*(const mat<4, T>& m1, const mat<4, T>& m2) {
  return mat<4, T>(m1 * m2[0], m1 * m2[1], m1 * m2[2], m1 * m2[3]);
}

template <typename T>
inline constexpr mat<4, T> operator/(const mat<4, T>& m, T scalar) {
  return mat<4, T>(m[0] / scalar, m[1] / scalar, m[2] / scalar, m[3] / scalar);
}

template <typename T>
inline constexpr mat<4, T> operator/(T scalar, const mat<4, T>& m) {
  return mat<4, T>(scalar / m[0], scalar / m[1], scalar / m[2], scalar / m[3]);
}

template <typename T>
inline constexpr mat<4, T>::col_t operator/(const mat<4, T>& m, const typename mat<4, T>::row_t& v) {
  return inverse(m) * v;
}

template <typename T>
inline constexpr mat<4, T>::row_t operator/(const typename mat<4, T>::col_t& v, const mat<4, T>& m) {
  return v * inverse(m);
}

template <typename T>
inline constexpr mat<4, T> operator/(const mat<4, T>& m1, const mat<4, T>& m2) {
  mat<4, T> copy(m1);
  return copy /= m2;
}

// Boolean operators
template <typename T>
inline constexpr bool operator==(const mat<4, T>& m1, const mat<4, T>& m2) {
  return m1[0] == m2[0] && m1[1] == m2[1] && m1[2] == m2[2] && m1[3] == m2[3];
}

template <typename T>
inline constexpr bool operator!=(const mat<4, T>& m1, const mat<4, T>& m2) {
  return !(m1 == m2);
}

}  // namespace mEn