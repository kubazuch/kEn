#pragma once

#include <mEn/fwd.hpp>

#if MEN_GLM
#include <glm/detail/qualifier.hpp>
#include <glm/vec3.hpp>
#endif

namespace mEn {

template <typename T>
struct vec<3, T> {
  static_assert(Scalar<T>, "mEn::vec<3, T> requires an arithmetic scalar type");

  using value_t = T;

#if MEN_GLM
  using glm_type = glm::vec<3, T, glm::defaultp>;
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

  [[nodiscard]] static constexpr length_t length() noexcept { return 3; }

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
  constexpr vec(T x, T y, T z) noexcept;

#if MEN_GLM
  template <Scalar U, glm::qualifier Q>
  constexpr vec(const glm::vec<3, U, Q>& v) noexcept;  // NOLINT(google-explicit-constructor)

  template <glm::qualifier Q = glm::defaultp>
  [[nodiscard]] constexpr operator glm::vec<3, T, Q>() const noexcept;  // NOLINT(google-explicit-constructor)
#endif

  // Conversion constructors
  template <Scalar X, Scalar Y, Scalar Z>
  constexpr vec(X x, Y y, Z z) noexcept;

  template <typename XY, Scalar Z>
  constexpr vec(const vec<2, XY>& xy, Z z) noexcept;
  template <Scalar X, typename YZ>
  constexpr vec(X x, const vec<2, YZ>& yz) noexcept;

  template <typename U>
  constexpr explicit vec(const vec<3, U>& v) noexcept;

  template <typename U>
  constexpr explicit vec(const vec<4, U>& v) noexcept;

  // Unary arithmetic operators
  template <typename U>
  constexpr vec& operator=(const vec<3, U>& v) noexcept;

  template <Scalar U>
  constexpr vec& operator+=(U scalar) noexcept;
  template <typename U>
  constexpr vec& operator+=(const vec<3, U>& v) noexcept;

  template <Scalar U>
  constexpr vec& operator-=(U scalar) noexcept;
  template <typename U>
  constexpr vec& operator-=(const vec<3, U>& v) noexcept;

  template <Scalar U>
  constexpr vec& operator*=(U scalar) noexcept;
  template <typename U>
  constexpr vec& operator*=(const vec<3, U>& v) noexcept;

  template <Scalar U>
  constexpr vec& operator/=(U scalar) noexcept;
  template <typename U>
  constexpr vec& operator/=(const vec<3, U>& v) noexcept;

  // Increment and decrement operators
  constexpr vec& operator++() noexcept;
  constexpr vec& operator--() noexcept;
  [[nodiscard]] constexpr vec operator++(int) noexcept;
  [[nodiscard]] constexpr vec operator--(int) noexcept;
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator+(vec<3, T> v) noexcept;
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator-(const vec<3, T>& v) noexcept;

// Scalar binary arithmetic operators
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<3, T> operator+(vec<3, T> v, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<3, T> operator+(U scalar, vec<3, T> v) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<3, T> operator-(vec<3, T> v, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<3, T> operator-(U scalar, const vec<3, T>& v) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<3, T> operator*(vec<3, T> v, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<3, T> operator*(U scalar, vec<3, T> v) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<3, T> operator/(vec<3, T> v, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr vec<3, T> operator/(U scalar, const vec<3, T>& v) noexcept;

// Vector binary arithmetic operators
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator+(vec<3, T> lhs, const vec<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator-(vec<3, T> lhs, const vec<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator*(vec<3, T> lhs, const vec<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator/(vec<3, T> lhs, const vec<3, U>& rhs) noexcept;

// Boolean operators
template <typename T, typename U>
[[nodiscard]] constexpr bool operator==(const vec<3, T>& v1, const vec<3, U>& v2) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr bool operator!=(const vec<3, T>& v1, const vec<3, U>& v2) noexcept;

}  // namespace mEn

#include "detail/vec3.inl"
