#pragma once

#include <mEn/fwd.hpp>

#if MEN_GLM
#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_quat.hpp>
#endif

namespace mEn {

template <typename T>
struct qua {
  static_assert(Scalar<T>, "mEn::qua<T> requires an arithmetic scalar type");

  using value_t = T;

#if MEN_GLM
  using glm_type = glm::qua<T, glm::defaultp>;
#endif

  T x, y, z, w;

  [[nodiscard]] static constexpr length_t length() noexcept { return 4; }

  [[nodiscard]] constexpr T& operator[](length_t i) noexcept;
  [[nodiscard]] constexpr const T& operator[](length_t i) const noexcept;

  // Basic construction and assignment
  constexpr qua() noexcept                      = default;
  constexpr qua(const qua& v) noexcept          = default;
  constexpr qua(qua&&) noexcept                 = default;
  constexpr qua& operator=(const qua&) noexcept = default;
  constexpr qua& operator=(qua&&) noexcept      = default;
  ~qua()                                        = default;

  constexpr qua(T s, const vec<3, T>& v) noexcept;
  constexpr qua(T w, T x, T y, T z) noexcept;

#if MEN_GLM
  template <Scalar U, glm::qualifier Q>
  constexpr qua(const glm::qua<U, Q>& q) noexcept;  // NOLINT(google-explicit-constructor)

  template <glm::qualifier Q = glm::defaultp>
  [[nodiscard]] constexpr operator glm::qua<T, Q>() const noexcept;  // NOLINT(google-explicit-constructor)
#endif

  // Conversion constructors
  template <typename U>
  constexpr explicit qua(const qua<U>& q) noexcept;

  [[nodiscard]] explicit operator mat<3, T>() const noexcept;
  [[nodiscard]] explicit operator mat<4, T>() const noexcept;

  qua(const vec<3, T>& u, const vec<3, T>& v) noexcept;

  explicit qua(const vec<3, T>& eulerAngles) noexcept;
  explicit qua(const mat<3, T>& m) noexcept;
  explicit qua(const mat<4, T>& m) noexcept;

  // Unary arithmetic operators
  template <typename U>
  constexpr qua& operator=(const qua<U>& q) noexcept;

  template <typename U>
  constexpr qua& operator+=(const qua<U>& q) noexcept;

  template <typename U>
  constexpr qua& operator-=(const qua<U>& q) noexcept;

  template <Scalar U>
  constexpr qua& operator*=(U scalar) noexcept;
  template <typename U>
  constexpr qua& operator*=(const qua<U>& q) noexcept;

  template <Scalar U>
  constexpr qua& operator/=(U scalar) noexcept;
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr qua<T> operator+(qua<T> q) noexcept;
template <typename T>
[[nodiscard]] constexpr qua<T> operator-(const qua<T>& q) noexcept;

// Scalar binary arithmetic operators
template <typename T, Scalar U>
[[nodiscard]] constexpr qua<T> operator*(qua<T> q, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr qua<T> operator*(U scalar, qua<T> q) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr qua<T> operator/(qua<T> q, U scalar) noexcept;

// Vector binary arithmetic operators
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator*(const qua<T>& q, const vec<3, U>& v) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator*(const vec<3, U>& v, const qua<T>& q) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator*(const qua<T>& q, const vec<4, U>& v) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator*(const vec<4, U>& v, const qua<T>& q) noexcept;

// Quaternion binary arithmetic operators
template <typename T, typename U>
[[nodiscard]] constexpr qua<T> operator+(qua<T> lhs, const qua<U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr qua<T> operator-(qua<T> lhs, const qua<U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr qua<T> operator*(qua<T> lhs, const qua<U>& rhs) noexcept;

// Boolean operators
template <typename T, typename U>
[[nodiscard]] constexpr bool operator==(const qua<T>& q1, const qua<U>& q2) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr bool operator!=(const qua<T>& q1, const qua<U>& q2) noexcept;

}  // namespace mEn

#include "detail/quat.inl"
