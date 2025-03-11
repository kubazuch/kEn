#pragma once

#ifdef MEN_GLM
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat3x3.hpp>

namespace mEn {

using Mat3 = ::glm::mat3;
using ::glm::inverse;

}  // namespace mEn
#else
#include "common.hpp"
#include "vec3.hpp"

namespace mEn {

template <typename T>
struct mat<3, T> {
  using value_t = T;
  using col_t = vec<3, T>;
  using row_t = vec<3, T>;

 private:
  col_t value_[3];

 public:
  [[nodiscard]] static constexpr int length() { return 3; }

  // Components
  [[nodiscard]] constexpr col_t& operator[](int i) noexcept;
  [[nodiscard]] constexpr const col_t& operator[](int i) const noexcept;

  // Implicit constructors
  constexpr mat()             = default;
  constexpr mat(const mat& m) = default;

  // Explicit constructors
  constexpr explicit mat(T scalar);
  constexpr mat(T x0, T y0, T z0,  //
                T x1, T y1, T z1,  //
                T x2, T y2, T z2);
  constexpr mat(const col_t& v0, const col_t& v1, const col_t& v2);

  // Conversion constructors
  template <typename X0, typename Y0, typename Z0,  //
            typename X1, typename Y1, typename Z1,  //
            typename X2, typename Y2, typename Z2>
  constexpr mat(X0 x0, Y0 y0, Z0 z0,  //
                X1 x1, Y1 y1, Z1 z1,  //
                X2 x2, Y2 y2, Z2 z2);
  template <typename U>
  constexpr explicit mat(const mat<4, U>& m);

  // Unary arithmetic operators
  constexpr mat<3, T>& operator=(const mat& m) = default;

  template <typename U>
  constexpr mat<3, T>& operator+=(U scalar);
  template <typename U>
  constexpr mat<3, T>& operator+=(const mat<3, U>& m);
  template <typename U>
  constexpr mat<3, T>& operator-=(U scalar);
  template <typename U>
  constexpr mat<3, T>& operator-=(const mat<3, U>& m);
  template <typename U>
  constexpr mat<3, T>& operator*=(U scalar);
  template <typename U>
  constexpr mat<3, T>& operator*=(const mat<3, U>& m);
  template <typename U>
  constexpr mat<3, T>& operator/=(U scalar);
  template <typename U>
  constexpr mat<3, T>& operator/=(const mat<3, U>& m);

  // Increment and decrement operators
  constexpr mat<3, T>& operator++();
  constexpr mat<3, T>& operator--();
  [[nodiscard]] constexpr mat<3, T> operator++(int);
  [[nodiscard]] constexpr mat<3, T> operator--(int);
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator+(const mat<3, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator-(const mat<3, T>& m);

// Binary arithmetic operators
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator+(const mat<3, T>& m, T scalar);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator+(T scalar, const mat<3, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator+(const mat<3, T>& m1, const mat<3, T>& m2);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator-(const mat<3, T>& m, T scalar);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator-(T scalar, const mat<3, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator-(const mat<3, T>& m1, const mat<3, T>& m2);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator*(const mat<3, T>& m, T scalar);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator*(T scalar, const mat<3, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<3, T>::col_t operator*(const mat<3, T>& m, const typename mat<3, T>::row_t& v);
template <typename T>
[[nodiscard]] constexpr mat<3, T>::row_t operator*(const typename mat<3, T>::col_t& v, const mat<3, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator*(const mat<3, T>& m1, const mat<3, T>& m2);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator/(const mat<3, T>& m, T scalar);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator/(T scalar, const mat<3, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<3, T>::col_t operator/(const mat<3, T>& m, const typename mat<3, T>::row_t& v);
template <typename T>
[[nodiscard]] constexpr mat<3, T>::row_t operator/(const typename mat<3, T>::col_t& v, const mat<3, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator/(const mat<3, T>& m1, const mat<3, T>& m2);

// Boolean operators
template <typename T>
[[nodiscard]] constexpr bool operator==(const mat<3, T>& m1, const mat<3, T>& m2);
template <typename T>
[[nodiscard]] constexpr bool operator!=(const mat<3, T>& m1, const mat<3, T>& m2);

using Mat3 = mat<3, float>;

}  // namespace mEn

#include "mat3.inl"

#endif