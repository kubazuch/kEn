#pragma once

#ifdef MEN_GLM
#include <glm/vec2.hpp>

namespace mEn {

using Vec2 = ::glm::vec2;

}
#else
#include "common.hpp"

namespace mEn {

template <typename T>
struct vec<2, T> {
  using value_t = T;

  union {
    T x, r, s;
  };
  union {
    T y, g, t;
  };

  [[nodiscard]] static constexpr int length() { return 2; }

  // Components
  [[nodiscard]] constexpr T& operator[](int i);
  [[nodiscard]] constexpr const T& operator[](int i) const;

  // Implicit constructors
  constexpr vec()             = default;
  constexpr vec(const vec& v) = default;

  // Explicit constructors
  constexpr explicit vec(T scalar);
  constexpr vec(T x, T y);

  // Conversion constructors
  template <typename X, typename Y>
  constexpr vec(X x, Y y);
  template <typename U>
  constexpr explicit vec(const vec<3, U>& v);
  template <typename U>
  constexpr explicit vec(const vec<4, U>& v);

  // Unary arithmetic operators
  constexpr vec<2, T>& operator=(const vec& v) = default;

  template <typename U>
  constexpr vec<2, T>& operator+=(U scalar);
  template <typename U>
  constexpr vec<2, T>& operator+=(const vec<2, U>& v);
  template <typename U>
  constexpr vec<2, T>& operator-=(U scalar);
  template <typename U>
  constexpr vec<2, T>& operator-=(const vec<2, U>& v);
  template <typename U>
  constexpr vec<2, T>& operator*=(U scalar);
  template <typename U>
  constexpr vec<2, T>& operator*=(const vec<2, U>& v);
  template <typename U>
  constexpr vec<2, T>& operator/=(U scalar);
  template <typename U>
  constexpr vec<2, T>& operator/=(const vec<2, U>& v);

  // Increment and decrement operators
  constexpr vec<2, T>& operator++();
  constexpr vec<2, T>& operator--();
  [[nodiscard]] constexpr vec<2, T> operator++(int);
  [[nodiscard]] constexpr vec<2, T> operator--(int);
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator+(const vec<2, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator-(const vec<2, T>& v);

// Binary arithmetic operators
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator+(const vec<2, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator+(T scalar, const vec<2, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator+(const vec<2, T>& v1, const vec<2, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator-(const vec<2, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator-(T scalar, const vec<2, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator-(const vec<2, T>& v1, const vec<2, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator*(const vec<2, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator*(T scalar, const vec<2, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator*(const vec<2, T>& v1, const vec<2, T>& v2);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator/(const vec<2, T>& v, T scalar);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator/(T scalar, const vec<2, T>& v);
template <typename T>
[[nodiscard]] constexpr vec<2, T> operator/(const vec<2, T>& v1, const vec<2, T>& v2);

// Boolean operators
template <typename T>
[[nodiscard]] constexpr bool operator==(const vec<2, T>& v1, const vec<2, T>& v2);
template <typename T>
[[nodiscard]] constexpr bool operator!=(const vec<2, T>& v1, const vec<2, T>& v2);

// Ostream
template <typename T>
std::ostream& operator<<(std::ostream& os, const vec<2, T>& v);

using Vec2 = vec<2, float>;

}  // namespace mEn

#include "vec2.inl"

#endif