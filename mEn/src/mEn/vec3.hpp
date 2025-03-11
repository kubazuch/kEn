#pragma once

#ifdef MEN_GLM
#include <glm/vec3.hpp>

namespace mEn {

using Vec3 = ::glm::vec3;

}
#else
#include "common.hpp"

namespace mEn {

template <typename T>
struct vec<3, T> {
  using value_t = T;

  union {
    T x, r, s;
  };
  union {
    T y, g, t;
  };
  union {
    T z, b, p;
  };

  [[nodiscard]] static constexpr int length() { return 3; }

  // Components
  [[nodiscard]] constexpr T& operator[](int i);
  [[nodiscard]] constexpr const T& operator[](int i) const;

  // Implicit constructors
  constexpr vec()             = default;
  constexpr vec(const vec& v) = default;

  // Explicit constructors
  constexpr explicit vec(T scalar);
  constexpr vec(T x, T y, T z);

  // Conversion constructors
  template <typename X, typename Y, typename Z>
  constexpr vec(X x, Y y, Z z);
  template <typename XY, typename Z>
  constexpr vec(const vec<2, XY>& xy, Z z);
  template <typename X, typename YZ>
  constexpr vec(X x, const vec<2, YZ>& yz);
  template <typename U>
  constexpr explicit vec(const vec<4, U>& v);

  // Unary arithmetic operators
  constexpr vec<3, T>& operator=(const vec& v) = default;

  template <typename U>
  constexpr vec<3, T>& operator+=(U scalar);
  template <typename U>
  constexpr vec<3, T>& operator+=(const vec<3, U>& v);
  template <typename U>
  constexpr vec<3, T>& operator-=(U scalar);
  template <typename U>
  constexpr vec<3, T>& operator-=(const vec<3, U>& v);
  template <typename U>
  constexpr vec<3, T>& operator*=(U scalar);
  template <typename U>
  constexpr vec<3, T>& operator*=(const vec<3, U>& v);
  template <typename U>
  constexpr vec<3, T>& operator/=(U scalar);
  template <typename U>
  constexpr vec<3, T>& operator/=(const vec<3, U>& v);

  // Increment and decrement operators
  constexpr vec<3, T>& operator++();
  constexpr vec<3, T>& operator--();
  [[nodiscard]] constexpr vec<3, T> operator++(int);
  [[nodiscard]] constexpr vec<3, T> operator--(int);
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator+(const vec<3, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator-(const vec<3, T>& v);

// Binary arithmetic operators
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator+(const vec<3, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator+(T scalar, const vec<3, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator+(const vec<3, T>& v1, const vec<3, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator-(const vec<3, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator-(T scalar, const vec<3, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator-(const vec<3, T>& v1, const vec<3, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator*(const vec<3, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator*(T scalar, const vec<3, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator*(const vec<3, T>& v1, const vec<3, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator/(const vec<3, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator/(T scalar, const vec<3, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator/(const vec<3, T>& v1, const vec<3, T>& v2);

// Boolean operators
template <typename T>
[[nodiscard]] constexpr bool operator==(const vec<3, T>& v1, const vec<3, T>& v2);
template <typename T>
[[nodiscard]] constexpr bool operator!=(const vec<3, T>& v1, const vec<3, T>& v2);

using Vec3 = vec<3, float>;

}  // namespace mEn

#include "vec3.inl"

#endif