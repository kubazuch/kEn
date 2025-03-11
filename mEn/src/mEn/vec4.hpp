#pragma once

#ifdef MEN_GLM
#include <glm/vec4.hpp>

namespace mEn {

using Vec4 = ::glm::vec4;

}
#else
#include "common.hpp"

namespace mEn {

template <typename T>
struct vec<4, T> {
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
  union {
    T w, a, q;
  };

  [[nodiscard]] static constexpr int length() { return 4; }

  // Components
  [[nodiscard]] constexpr T& operator[](int i);
  [[nodiscard]] constexpr const T& operator[](int i) const;

  // Implicit constructors
  constexpr vec()             = default;
  constexpr vec(const vec& v) = default;

  // Explicit constructors
  constexpr explicit vec(T scalar);
  constexpr vec(T x, T y, T z, T w);

  // Conversion constructors
  template <typename X, typename Y, typename Z, typename W>
  constexpr vec(X x, Y y, Z z, W w);
  template <typename XY, typename Z, typename W>
  constexpr vec(const vec<2, XY>& xy, Z z, W w);
  template <typename X, typename YZ, typename W>
  constexpr vec(X x, const vec<2, YZ>& yz, W w);
  template <typename X, typename Y, typename ZW>
  constexpr vec(X x, Y y, const vec<2, ZW>& zw);
  template <typename X, typename YZW>
  constexpr vec(X x, const vec<3, YZW>& yzw);
  template <typename XYZ, typename W>
  constexpr vec(const vec<3, XYZ>& xyz, W w);
  template <typename XY, typename ZW>
  constexpr vec(const vec<2, XY>& xy, const vec<2, ZW>& zw);

  // Unary arithmetic operators
  constexpr vec<4, T>& operator=(const vec& v) = default;

  template <typename U>
  constexpr vec<4, T>& operator+=(U scalar);
  template <typename U>
  constexpr vec<4, T>& operator+=(const vec<4, U>& v);
  template <typename U>
  constexpr vec<4, T>& operator-=(U scalar);
  template <typename U>
  constexpr vec<4, T>& operator-=(const vec<4, U>& v);
  template <typename U>
  constexpr vec<4, T>& operator*=(U scalar);
  template <typename U>
  constexpr vec<4, T>& operator*=(const vec<4, U>& v);
  template <typename U>
  constexpr vec<4, T>& operator/=(U scalar);
  template <typename U>
  constexpr vec<4, T>& operator/=(const vec<4, U>& v);

  // Increment and decrement operators
  constexpr vec<4, T>& operator++();
  constexpr vec<4, T>& operator--();
  [[nodiscard]] constexpr vec<4, T> operator++(int);
  [[nodiscard]] constexpr vec<4, T> operator--(int);
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator+(const vec<4, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator-(const vec<4, T>& v);

// Binary arithmetic operators
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator+(const vec<4, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator+(T scalar, const vec<4, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator+(const vec<4, T>& v1, const vec<4, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator-(const vec<4, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator-(T scalar, const vec<4, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator-(const vec<4, T>& v1, const vec<4, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator*(const vec<4, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator*(T scalar, const vec<4, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator*(const vec<4, T>& v1, const vec<4, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator/(const vec<4, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator/(T scalar, const vec<4, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator/(const vec<4, T>& v1, const vec<4, T>& v2);

// Boolean operators
template <typename T>
[[nodiscard]] constexpr bool operator==(const vec<4, T>& v1, const vec<4, T>& v2);
template <typename T>
[[nodiscard]] constexpr bool operator!=(const vec<4, T>& v1, const vec<4, T>& v2);

using Vec4 = vec<4, float>;

}  // namespace mEn

#include "vec4.inl"

#endif