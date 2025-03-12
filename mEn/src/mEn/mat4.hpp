#pragma once

#ifdef MEN_GLM
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/mat4x4.hpp>

namespace mEn {

using Mat4 = ::glm::mat4;
using ::glm::decompose;
using ::glm::inverse;
using ::glm::rotate;
using ::glm::scale;
using ::glm::translate;

}  // namespace mEn
#else
#include "common.hpp"
#include "vec4.hpp"

namespace mEn {

template <typename T>
struct mat<4, T> {
  using value_t = T;
  using col_t   = vec<4, T>;
  using row_t   = vec<4, T>;

 private:
  col_t value_[4];

 public:
  [[nodiscard]] static constexpr int length() { return 4; }

  // Components
  [[nodiscard]] constexpr col_t& operator[](int i) noexcept;
  [[nodiscard]] constexpr const col_t& operator[](int i) const noexcept;

  // Implicit constructors
  constexpr mat()             = default;
  constexpr mat(const mat& m) = default;

  // Explicit constructors
  constexpr explicit mat(T scalar);
  constexpr mat(T x0, T y0, T z0, T w0,  //
                T x1, T y1, T z1, T w1,  //
                T x2, T y2, T z2, T w2,  //
                T x3, T y3, T z3, T w3);
  constexpr mat(const col_t& v0, const col_t& v1, const col_t& v2, const col_t& v3);

  // Conversion constructors
  template <typename X0, typename Y0, typename Z0, typename W0,  //
            typename X1, typename Y1, typename Z1, typename W1,  //
            typename X2, typename Y2, typename Z2, typename W2,  //
            typename X3, typename Y3, typename Z3, typename W3>
  constexpr mat(X0 x0, Y0 y0, Z0 z0, W0 w0,  //
                X1 x1, Y1 y1, Z1 z1, W1 w1,  //
                X2 x2, Y2 y2, Z2 z2, W2 w2,  //
                X3 x3, Y3 y3, Z3 z3, W3 w3);
  template <typename U>
  constexpr explicit mat(const mat<3, U>& m);

  // Unary arithmetic operators
  constexpr mat<4, T>& operator=(const mat& m) = default;

  template <typename U>
  constexpr mat<4, T>& operator+=(U scalar);
  template <typename U>
  constexpr mat<4, T>& operator+=(const mat<4, U>& m);
  template <typename U>
  constexpr mat<4, T>& operator-=(U scalar);
  template <typename U>
  constexpr mat<4, T>& operator-=(const mat<4, U>& m);
  template <typename U>
  constexpr mat<4, T>& operator*=(U scalar);
  template <typename U>
  constexpr mat<4, T>& operator*=(const mat<4, U>& m);
  template <typename U>
  constexpr mat<4, T>& operator/=(U scalar);
  template <typename U>
  constexpr mat<4, T>& operator/=(const mat<4, U>& m);

  // Increment and decrement operators
  constexpr mat<4, T>& operator++();
  constexpr mat<4, T>& operator--();
  [[nodiscard]] constexpr mat<4, T> operator++(int);
  [[nodiscard]] constexpr mat<4, T> operator--(int);
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator+(const mat<4, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator-(const mat<4, T>& m);

// Binary arithmetic operators
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator+(const mat<4, T>& m, T scalar);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator+(T scalar, const mat<4, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator+(const mat<4, T>& m1, const mat<4, T>& m2);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator-(const mat<4, T>& m, T scalar);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator-(T scalar, const mat<4, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator-(const mat<4, T>& m1, const mat<4, T>& m2);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator*(const mat<4, T>& m, T scalar);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator*(T scalar, const mat<4, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<4, T>::col_t operator*(const mat<4, T>& m, const typename mat<4, T>::row_t& v);
template <typename T>
[[nodiscard]] constexpr mat<4, T>::row_t operator*(const typename mat<4, T>::col_t& v, const mat<4, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator*(const mat<4, T>& m1, const mat<4, T>& m2);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator/(const mat<4, T>& m, T scalar);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator/(T scalar, const mat<4, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<4, T>::col_t operator/(const mat<4, T>& m, const typename mat<4, T>::row_t& v);
template <typename T>
[[nodiscard]] constexpr mat<4, T>::row_t operator/(const typename mat<4, T>::col_t& v, const mat<4, T>& m);
template <typename T>
[[nodiscard]] constexpr mat<4, T> operator/(const mat<4, T>& m1, const mat<4, T>& m2);

// Boolean operators
template <typename T>
[[nodiscard]] constexpr bool operator==(const mat<4, T>& m1, const mat<4, T>& m2);
template <typename T>
[[nodiscard]] constexpr bool operator!=(const mat<4, T>& m1, const mat<4, T>& m2);

using Mat4 = mat<4, float>;

}  // namespace mEn

#include "mat4.inl"

#endif