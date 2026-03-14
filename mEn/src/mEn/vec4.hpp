#pragma once

#include <mEn/fwd.hpp>

#if MEN_GLM
#include <glm/detail/qualifier.hpp>
#include <glm/vec4.hpp>
#endif

namespace mEn {

template <typename T>
struct vec<4, T> {
  static_assert(Scalar<T>, "mEn::vec<4, T> requires an arithmetic scalar type");

  using value_t = T;

#if MEN_GLM
  using glm_type = glm::vec<4, T, glm::defaultp>;
#endif

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

  [[nodiscard]] static constexpr length_t length() noexcept { return 4; }

  [[nodiscard]] constexpr T& operator[](length_t i) noexcept;
  [[nodiscard]] constexpr const T& operator[](length_t i) const noexcept;

  // Basic construction and assignment
  constexpr vec() noexcept                      = default;
  constexpr vec(const vec&) noexcept            = default;
  constexpr vec(vec&&) noexcept                 = default;
  constexpr vec& operator=(const vec&) noexcept = default;
  constexpr vec& operator=(vec&&) noexcept      = default;
  ~vec()                                        = default;

  constexpr explicit vec(T scalar) noexcept;
  constexpr vec(T x, T y, T z, T w) noexcept;

#if MEN_GLM
  template <Scalar U, glm::qualifier Q>
  constexpr vec(const glm::vec<4, U, Q>& v) noexcept;  // NOLINT(google-explicit-constructor)

  template <glm::qualifier Q = glm::defaultp>
  [[nodiscard]] constexpr operator glm::vec<4, T, Q>() const noexcept;  // NOLINT(google-explicit-constructor)
#endif

  // Conversion constructors
  template <Scalar X, Scalar Y, Scalar Z, Scalar W>
  constexpr vec(X x, Y y, Z z, W w) noexcept;

  template <typename XY, Scalar Z, Scalar W>
  constexpr vec(const vec<2, XY>& xy, Z z, W w) noexcept;
  template <Scalar X, typename YZ, Scalar W>
  constexpr vec(X x, const vec<2, YZ>& yz, W w) noexcept;
  template <Scalar X, Scalar Y, typename ZW>
  constexpr vec(X x, Y y, const vec<2, ZW>& zw) noexcept;

  template <Scalar X, typename YZW>
  constexpr vec(X x, const vec<3, YZW>& yzw) noexcept;
  template <typename XYZ, Scalar W>
  constexpr vec(const vec<3, XYZ>& xyz, W w) noexcept;

  template <typename XY, typename ZW>
  constexpr vec(const vec<2, XY>& xy, const vec<2, ZW>& zw) noexcept;

  template <typename U>
  constexpr explicit vec(const vec<4, U>& v) noexcept;

  // Unary arithmetic operators
  template <typename U>
  constexpr vec& operator=(const vec<4, U>& v) noexcept;

  template <Scalar U>
  constexpr vec& operator+=(U scalar) noexcept;
  template <typename U>
  constexpr vec& operator+=(const vec<4, U>& v) noexcept;

  template <Scalar U>
  constexpr vec& operator-=(U scalar) noexcept;
  template <typename U>
  constexpr vec& operator-=(const vec<4, U>& v) noexcept;

  template <Scalar U>
  constexpr vec& operator*=(U scalar) noexcept;
  template <typename U>
  constexpr vec& operator*=(const vec<4, U>& v) noexcept;

  template <Scalar U>
  constexpr vec& operator/=(U scalar) noexcept;
  template <typename U>
  constexpr vec& operator/=(const vec<4, U>& v) noexcept;

  // Increment and decrement operators
  constexpr vec& operator++() noexcept;
  constexpr vec& operator--() noexcept;
  [[nodiscard]] constexpr vec operator++(int) noexcept;
  [[nodiscard]] constexpr vec operator--(int) noexcept;
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator+(vec<4, T> v) noexcept;
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator-(const vec<4, T>& v) noexcept;

// Scalar binary arithmetic operators
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<4, T> operator+(vec<4, T> v, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<4, T> operator+(U scalar, vec<4, T> v) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<4, T> operator-(vec<4, T> v, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<4, T> operator-(U scalar, const vec<4, T>& v) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<4, T> operator*(vec<4, T> v, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<4, T> operator*(U scalar, vec<4, T> v) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<4, T> operator/(vec<4, T> v, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<4, T> operator/(U scalar, const vec<4, T>& v) noexcept;

// Vector binary arithmetic operators
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator+(vec<4, T> lhs, const vec<4, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator-(vec<4, T> lhs, const vec<4, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator*(vec<4, T> lhs, const vec<4, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator/(vec<4, T> lhs, const vec<4, U>& rhs) noexcept;

// Boolean operators
template <typename T, typename U>
[[nodiscard]] constexpr bool operator==(const vec<4, T>& v1, const vec<4, U>& v2) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr bool operator!=(const vec<4, T>& v1, const vec<4, U>& v2) noexcept;

}  // namespace mEn

#include "detail/vec4.inl"
