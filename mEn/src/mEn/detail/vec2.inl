#pragma once
#include "vec2.hpp"

namespace mEn {

// Explicit constructors
template <typename T>
inline constexpr vec<2, T>::vec(T scalar) : x(scalar), y(scalar) {}
template <typename T>
inline constexpr vec<2, T>::vec(T _x, T _y) : x(_x), y(_y) {}

// Conversion constructors
template <typename T>
template <typename X, typename Y>
inline constexpr vec<2, T>::vec(X _x, Y _y) : x(static_cast<float>(_x)), y(static_cast<float>(_y)) {}

template <typename T>
template <typename U>
inline constexpr vec<2, T>::vec(const vec<3, U>& v) : x(static_cast<T>(v.x)), y(static_cast<U>(v.y)) {}

template <typename T>
template <typename U>
inline constexpr vec<2, T>::vec(const vec<4, U>& v) : x(static_cast<T>(v.x)), y(static_cast<U>(v.y)) {}

// Components
template <typename T>
inline constexpr T& vec<2, T>::operator[](int i) {
  assert(i >= 0 && i < length());
  switch (i) {
    default:
    case 0:
      return x;
    case 1:
      return y;
  }
}

template <typename T>
inline constexpr const T& vec<2, T>::operator[](int i) const {
  assert(i >= 0 && i < length());
  switch (i) {
    default:
    case 0:
      return x;
    case 1:
      return y;
  }
}

// Unary arithmetic operators
template <typename T>
template <typename U>
inline constexpr vec<2, T>& vec<2, T>::operator+=(U scalar) {
  this->x += static_cast<T>(scalar);
  this->y += static_cast<T>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<2, T>& vec<2, T>::operator+=(const vec<2, U>& v) {
  this->x += static_cast<T>(v.x);
  this->y += static_cast<T>(v.y);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<2, T>& vec<2, T>::operator-=(U scalar) {
  this->x -= static_cast<float>(scalar);
  this->y -= static_cast<float>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<2, T>& vec<2, T>::operator-=(const vec<2, U>& v) {
  this->x -= static_cast<T>(v.x);
  this->y -= static_cast<T>(v.y);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<2, T>& vec<2, T>::operator*=(U scalar) {
  this->x *= static_cast<float>(scalar);
  this->y *= static_cast<float>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<2, T>& vec<2, T>::operator*=(const vec<2, U>& v) {
  this->x *= static_cast<T>(v.x);
  this->y *= static_cast<T>(v.y);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<2, T>& vec<2, T>::operator/=(U scalar) {
  this->x /= static_cast<float>(scalar);
  this->y /= static_cast<float>(scalar);
  return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<2, T>& vec<2, T>::operator/=(const vec<2, U>& v) {
  this->x /= static_cast<T>(v.x);
  this->y /= static_cast<T>(v.y);
  return *this;
}

// Increment and decrement operators
template <typename T>
inline constexpr vec<2, T>& vec<2, T>::operator++() {
  ++this->x;
  ++this->y;
  return *this;
}

template <typename T>
inline constexpr vec<2, T>& vec<2, T>::operator--() {
  --this->x;
  --this->y;
  return *this;
}

template <typename T>
inline constexpr vec<2, T> vec<2, T>::operator++(int) {
  vec<2, T> result(*this);
  ++*this;
  return result;
}

template <typename T>
inline constexpr vec<2, T> vec<2, T>::operator--(int) {
  vec<2, T> result(*this);
  --*this;
  return result;
}

// Unary operators
template <typename T>
inline constexpr vec<2, T> operator+(const vec<2, T>& v) {
  return v;
}

template <typename T>
inline constexpr vec<2, T> operator-(const vec<2, T>& v) {
  return vec<2, T>(-v.x, -v.y);
}

// Binary arithmetic operatorstemplate <typename T>
template <typename T>
inline constexpr vec<2, T> operator+(const vec<2, T>& v, T scalar) {
  return vec<2, T>(v.x + scalar, v.y + scalar);
}

template <typename T>
inline constexpr vec<2, T> operator+(T scalar, const vec<2, T>& v) {
  return vec<2, T>(scalar + v.x, scalar + v.y);
}

template <typename T>
inline constexpr vec<2, T> operator+(const vec<2, T>& v1, const vec<2, T>& v2) {
  return vec<2, T>(v1.x + v2.x, v1.y + v2.y);
}

template <typename T>
inline constexpr vec<2, T> operator-(const vec<2, T>& v, T scalar) {
  return vec<2, T>(v.x - scalar, v.y - scalar);
}

template <typename T>
inline constexpr vec<2, T> operator-(T scalar, const vec<2, T>& v) {
  return vec<2, T>(scalar - v.x, scalar - v.y);
}

template <typename T>
inline constexpr vec<2, T> operator-(const vec<2, T>& v1, const vec<2, T>& v2) {
  return vec<2, T>(v1.x - v2.x, v1.y - v2.y);
}

template <typename T>
inline constexpr vec<2, T> operator*(const vec<2, T>& v, T scalar) {
  return vec<2, T>(v.x * scalar, v.y * scalar);
}

template <typename T>
inline constexpr vec<2, T> operator*(T scalar, const vec<2, T>& v) {
  return vec<2, T>(scalar * v.x, scalar * v.y);
}

template <typename T>
inline constexpr vec<2, T> operator*(const vec<2, T>& v1, const vec<2, T>& v2) {
  return vec<2, T>(v1.x * v2.x, v1.y * v2.y);
}

template <typename T>
inline constexpr vec<2, T> operator/(const vec<2, T>& v, T scalar) {
  return vec<2, T>(v.x / scalar, v.y / scalar);
}

template <typename T>
inline constexpr vec<2, T> operator/(T scalar, const vec<2, T>& v) {
  return vec<2, T>(scalar / v.x, scalar / v.y);
}

template <typename T>
inline constexpr vec<2, T> operator/(const vec<2, T>& v1, const vec<2, T>& v2) {
  return vec<2, T>(v1.x / v2.x, v1.y / v2.y);
}

// Boolean operators
template <typename T>
inline constexpr bool operator==(const vec<2, T>& v1, const vec<2, T>& v2) {
  // TODO(C++23): Implement epsilon comparison
  return v1.x == v2.x && v1.y == v2.y;
}

template <typename T>
inline constexpr bool operator!=(const vec<2, T>& v1, const vec<2, T>& v2) {
  return !(v1 == v2);
}

// Ostream
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const vec<2, T>& v) {
  return os << "(" << v.x << ", " << v.y << ")";
}

}  // namespace mEn