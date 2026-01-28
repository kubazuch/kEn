#pragma once
#include "vec4.hpp"

namespace mEn {

// Explicit constructors
template <typename T>
inline constexpr vec<4, T>::vec(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
template <typename T>
inline constexpr vec<4, T>::vec(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

// Conversion constructors
template <typename T>
template <typename X, typename Y, typename Z, typename W>
inline constexpr vec<4, T>::vec(X _x, Y _y, Z _z, W _w)
    : x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(_w)) {}

template <typename T>
template <typename XY, typename Z, typename W>
inline constexpr vec<4, T>::vec(const vec<2, XY>& _xy, Z _z, W _w)
    : x(static_cast<T>(_xy.x)), y(static_cast<T>(_xy.y)), z(static_cast<T>(_z)), w(static_cast<T>(_w)) {}

template <typename T>
template <typename X, typename YZ, typename W>
inline constexpr vec<4, T>::vec(X _x, const vec<2, YZ>& _yz, W _w)
    : x(static_cast<T>(_x)), y(static_cast<T>(_yz.x)), z(static_cast<T>(_yz.y)), w(static_cast<T>(_w)) {}

template <typename T>
template <typename X, typename Y, typename ZW>
inline constexpr vec<4, T>::vec(X _x, Y _y, const vec<2, ZW>& _zw)
    : x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_zw.x)), w(static_cast<T>(_zw.y)) {}

template <typename T>
template <typename X, typename YZW>
inline constexpr vec<4, T>::vec(X _x, const vec<3, YZW>& _yzw)
    : x(static_cast<T>(_x)), y(static_cast<T>(_yzw.x)), z(static_cast<T>(_yzw.y)), w(static_cast<T>(_yzw.z)) {}

template <typename T>
template <typename XYZ, typename W>
inline constexpr vec<4, T>::vec(const vec<3, XYZ>& _xyz, W _w)
    : x(static_cast<T>(_xyz.x)), y(static_cast<T>(_xyz.y)), z(static_cast<T>(_xyz.z)), w(static_cast<T>(_w)) {}

template <typename T>
template <typename XY, typename ZW>
inline constexpr vec<4, T>::vec(const vec<2, XY>& _xy, const vec<2, ZW>& _zw)
    : x(static_cast<T>(_xy.x)), y(static_cast<T>(_xy.y)), z(static_cast<T>(_zw.x)), w(static_cast<T>(_zw.y)) {}

// Components
template <typename T>
inline constexpr T& vec<4, T>::operator[](int i) {
  assert(i >= 0 && i < length());
  switch (i) {
    default:
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
  }
}

template <typename T>
inline constexpr const T& vec<4, T>::operator[](int i) const {
  assert(i >= 0 && i < length());
  switch (i) {
    default:
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
  }
}

// Unary arithmetic operators
template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator+=(U scalar) {
  this->x += static_cast<T>(scalar);
  this->y += static_cast<T>(scalar);
  this->z += static_cast<T>(scalar);
  this->w += static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator+=(const vec<4, U>& v) {
  this->x += static_cast<T>(v.x);
  this->y += static_cast<T>(v.y);
  this->z += static_cast<T>(v.z);
  this->w += static_cast<T>(v.w);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator-=(U scalar) {
  this->x -= static_cast<T>(scalar);
  this->y -= static_cast<T>(scalar);
  this->z -= static_cast<T>(scalar);
  this->w -= static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator-=(const vec<4, U>& v) {
  this->x -= static_cast<T>(v.x);
  this->y -= static_cast<T>(v.y);
  this->z -= static_cast<T>(v.z);
  this->w -= static_cast<T>(v.w);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator*=(U scalar) {
  this->x *= static_cast<T>(scalar);
  this->y *= static_cast<T>(scalar);
  this->z *= static_cast<T>(scalar);
  this->w *= static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator*=(const vec<4, U>& v) {
  this->x *= static_cast<T>(v.x);
  this->y *= static_cast<T>(v.y);
  this->z *= static_cast<T>(v.z);
  this->w *= static_cast<T>(v.w);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator/=(U scalar) {
  this->x /= static_cast<T>(scalar);
  this->y /= static_cast<T>(scalar);
  this->z /= static_cast<T>(scalar);
  this->w /= static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator/=(const vec<4, U>& v) {
  this->x /= static_cast<T>(v.x);
  this->y /= static_cast<T>(v.y);
  this->z /= static_cast<T>(v.z);
  this->w /= static_cast<T>(v.w);
  return *this;
}

// Increment and decrement operators
template <typename T>
inline constexpr vec<4, T>& vec<4, T>::operator++() {
  ++this->x;
  ++this->y;
  ++this->z;
  ++this->w;
  return *this;
}

template <typename T>
inline constexpr vec<4, T>& vec<4, T>::operator--() {
  --this->x;
  --this->y;
  --this->z;
  --this->w;
  return *this;
}

template <typename T>
inline constexpr vec<4, T> vec<4, T>::operator++(int) {
  vec<4, T> result(*this);
  ++*this;
  return result;
}

template <typename T>
inline constexpr vec<4, T> vec<4, T>::operator--(int) {
  vec<4, T> result(*this);
  --*this;
  return result;
}

// Unary operators
template <typename T>
inline constexpr vec<4, T> operator+(const vec<4, T>& v) {
  return v;
}

template <typename T>
inline constexpr vec<4, T> operator-(const vec<4, T>& v) {
  return vec<4, T>(-v.x, -v.y, -v.z, -v.w);
}

// Binary arithmetic operators
template <typename T>
inline constexpr vec<4, T> operator+(const vec<4, T>& v, T scalar) {
  return vec<4, T>(v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar);
}

template <typename T>
inline constexpr vec<4, T> operator+(T scalar, const vec<4, T>& v) {
  return vec<4, T>(scalar + v.x, scalar + v.y, scalar + v.z, scalar + v.w);
}

template <typename T>
inline constexpr vec<4, T> operator+(const vec<4, T>& v1, const vec<4, T>& v2) {
  return vec<4, T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

template <typename T>
inline constexpr vec<4, T> operator-(const vec<4, T>& v, T scalar) {
  return vec<4, T>(v.x - scalar, v.y - scalar, v.z - scalar, v.w - scalar);
}

template <typename T>
inline constexpr vec<4, T> operator-(T scalar, const vec<4, T>& v) {
  return vec<4, T>(scalar - v.x, scalar - v.y, scalar - v.z, scalar - v.w);
}

template <typename T>
inline constexpr vec<4, T> operator-(const vec<4, T>& v1, const vec<4, T>& v2) {
  return vec<4, T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

template <typename T>
inline constexpr vec<4, T> operator*(const vec<4, T>& v, T scalar) {
  return vec<4, T>(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

template <typename T>
inline constexpr vec<4, T> operator*(T scalar, const vec<4, T>& v) {
  return vec<4, T>(scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.w);
}

template <typename T>
inline constexpr vec<4, T> operator*(const vec<4, T>& v1, const vec<4, T>& v2) {
  return vec<4, T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

template <typename T>
inline constexpr vec<4, T> operator/(const vec<4, T>& v, T scalar) {
  return vec<4, T>(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

template <typename T>
inline constexpr vec<4, T> operator/(T scalar, const vec<4, T>& v) {
  return vec<4, T>(scalar / v.x, scalar / v.y, scalar / v.z, scalar / v.w);
}

template <typename T>
inline constexpr vec<4, T> operator/(const vec<4, T>& v1, const vec<4, T>& v2) {
  return vec<4, T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

// Boolean operators
template <typename T>
inline constexpr bool operator==(const vec<4, T>& v1, const vec<4, T>& v2) {
  // TODO(C++23): Implement epsilon comparison
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
}

template <typename T>
inline constexpr bool operator!=(const vec<4, T>& v1, const vec<4, T>& v2) {
  return !(v1 == v2);
}

// Ostream
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const vec<4, T>& v) {
  return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}

}  // namespace mEn