#pragma once
#include "vec3.hpp"

namespace mEn {

// Explicit constructors
template <typename T>
inline constexpr vec<3, T>::vec(T scalar) : x(scalar), y(scalar), z(scalar) {}
template <typename T>
inline constexpr vec<3, T>::vec(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

// Conversion constructors
template <typename T>
template <typename X, typename Y, typename Z>
inline constexpr vec<3, T>::vec(X _x, Y _y, Z _z)
    : x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)) {}

template <typename T>
template <typename XY, typename Z>
inline constexpr vec<3, T>::vec(const vec<2, XY>& _xy, Z _z)
    : x(static_cast<T>(_xy.x)), y(static_cast<T>(_xy.y)), z(static_cast<T>(_z)) {}
template <typename T>
template <typename X, typename YZ>
inline constexpr vec<3, T>::vec(X _x, const vec<2, YZ>& _yz)
    : x(static_cast<T>(_x)), y(static_cast<T>(_yz.x)), z(static_cast<T>(_yz.y)) {}
template <typename T>
template <typename U>
inline constexpr vec<3, T>::vec(const vec<4, U>& v)
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}

// Components
template <typename T>
inline constexpr T& vec<3, T>::operator[](int i) {
  assert(i >= 0 && i < length());
  switch (i) {
    default:
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
  }
}

template <typename T>
inline constexpr const T& vec<3, T>::operator[](int i) const {
  assert(i >= 0 && i < length());
  switch (i) {
    default:
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
  }
}

// Unary arithmetic operators
template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator+=(U scalar) {
  this->x += static_cast<T>(scalar);
  this->y += static_cast<T>(scalar);
  this->z += static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator+=(const vec<3, U>& v) {
  this->x += static_cast<T>(v.x);
  this->y += static_cast<T>(v.y);
  this->z += static_cast<T>(v.z);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator-=(U scalar) {
  this->x -= static_cast<T>(scalar);
  this->y -= static_cast<T>(scalar);
  this->z -= static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator-=(const vec<3, U>& v) {
  this->x -= static_cast<T>(v.x);
  this->y -= static_cast<T>(v.y);
  this->z -= static_cast<T>(v.z);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator*=(U scalar) {
  this->x *= static_cast<T>(scalar);
  this->y *= static_cast<T>(scalar);
  this->z *= static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator*=(const vec<3, U>& v) {
  this->x *= static_cast<T>(v.x);
  this->y *= static_cast<T>(v.y);
  this->z *= static_cast<T>(v.z);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator/=(U scalar) {
  this->x /= static_cast<T>(scalar);
  this->y /= static_cast<T>(scalar);
  this->z /= static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator/=(const vec<3, U>& v) {
  this->x /= static_cast<T>(v.x);
  this->y /= static_cast<T>(v.y);
  this->z /= static_cast<T>(v.z);
  return *this;
}

// Increment and decrement operators
template <typename T>
inline constexpr vec<3, T>& vec<3, T>::operator++() {
  ++this->x;
  ++this->y;
  ++this->z;
  return *this;
}

template <typename T>
inline constexpr vec<3, T>& vec<3, T>::operator--() {
  --this->x;
  --this->y;
  --this->z;
  return *this;
}

template <typename T>
inline constexpr vec<3, T> vec<3, T>::operator++(int) {
  vec<3, T> result(*this);
  ++*this;
  return result;
}

template <typename T>
inline constexpr vec<3, T> vec<3, T>::operator--(int) {
  vec<3, T> result(*this);
  --*this;
  return result;
}

// Unary operators
template <typename T>
inline constexpr vec<3, T> operator+(const vec<3, T>& v) {
  return v;
}

template <typename T>
inline constexpr vec<3, T> operator-(const vec<3, T>& v) {
  return vec<3, T>(-v.x, -v.y, -v.z);
}

// Binary arithmetic operatorstemplate <typename T>
template <typename T>
inline constexpr vec<3, T> operator+(const vec<3, T>& v, T scalar) {
  return vec<3, T>(v.x + scalar, v.y + scalar, v.z + scalar);
}

template <typename T>
inline constexpr vec<3, T> operator+(T scalar, const vec<3, T>& v) {
  return vec<3, T>(scalar + v.x, scalar + v.y, scalar + v.z);
}

template <typename T>
inline constexpr vec<3, T> operator+(const vec<3, T>& v1, const vec<3, T>& v2) {
  return vec<3, T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <typename T>
inline constexpr vec<3, T> operator-(const vec<3, T>& v, T scalar) {
  return vec<3, T>(v.x - scalar, v.y - scalar, v.z - scalar);
}

template <typename T>
inline constexpr vec<3, T> operator-(T scalar, const vec<3, T>& v) {
  return vec<3, T>(scalar - v.x, scalar - v.y, scalar - v.z);
}

template <typename T>
inline constexpr vec<3, T> operator-(const vec<3, T>& v1, const vec<3, T>& v2) {
  return vec<3, T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template <typename T>
inline constexpr vec<3, T> operator*(const vec<3, T>& v, T scalar) {
  return vec<3, T>(v.x * scalar, v.y * scalar, v.z * scalar);
}

template <typename T>
inline constexpr vec<3, T> operator*(T scalar, const vec<3, T>& v) {
  return vec<3, T>(scalar * v.x, scalar * v.y, scalar * v.z);
}

template <typename T>
inline constexpr vec<3, T> operator*(const vec<3, T>& v1, const vec<3, T>& v2) {
  return vec<3, T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

template <typename T>
inline constexpr vec<3, T> operator/(const vec<3, T>& v, T scalar) {
  return vec<3, T>(v.x / scalar, v.y / scalar, v.z / scalar);
}

template <typename T>
inline constexpr vec<3, T> operator/(T scalar, const vec<3, T>& v) {
  return vec<3, T>(scalar / v.x, scalar / v.y, scalar / v.z);
}

template <typename T>
inline constexpr vec<3, T> operator/(const vec<3, T>& v1, const vec<3, T>& v2) {
  return vec<3, T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

// Boolean operators
template <typename T>
inline constexpr bool operator==(const vec<3, T>& v1, const vec<3, T>& v2) {
  // TODO(C++23): Implement epsilon comparison
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

template <typename T>
inline constexpr bool operator!=(const vec<3, T>& v1, const vec<3, T>& v2) {
  return !(v1 == v2);
}

}  // namespace mEn