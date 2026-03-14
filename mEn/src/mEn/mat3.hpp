#pragma once

#include <mEn/fwd.hpp>

#if MEN_GLM
#include <glm/detail/qualifier.hpp>
#include <glm/mat3x3.hpp>
#endif

#include <mEn/vec3.hpp>

namespace mEn {

template <typename T>
struct mat<3, T> {
  static_assert(Scalar<T>, "mEn::mat<3, T> requires an arithmetic scalar type");

  using value_t = T;

  using col_t = vec<3, T>;
  using row_t = vec<3, T>;

#if MEN_GLM
  using glm_type = glm::mat<3, 3, T, glm::defaultp>;
#endif

 private:
  col_t value_[3]{};

 public:
  [[nodiscard]] static constexpr length_t length() noexcept { return 3; }

  [[nodiscard]] constexpr col_t& operator[](length_t i) noexcept;
  [[nodiscard]] constexpr const col_t& operator[](length_t i) const noexcept;

  // Basic construction and assignment
  constexpr mat() noexcept                      = default;
  constexpr mat(const mat& m) noexcept          = default;
  constexpr mat(mat&& m) noexcept               = default;
  constexpr mat& operator=(const mat&) noexcept = default;
  constexpr mat& operator=(mat&&) noexcept      = default;
  ~mat()                                        = default;

  // Explicit constructors
  constexpr explicit mat(T scalar) noexcept;
  constexpr mat(T x0, T y0, T z0, T x1, T y1, T z1, T x2, T y2, T z2) noexcept;
  constexpr mat(const col_t& v0, const col_t& v1, const col_t& v2) noexcept;

#if MEN_GLM
  template <Scalar U, glm::qualifier Q>
  constexpr mat(const glm::mat<3, 3, U, Q>& m) noexcept;  // NOLINT(google-explicit-constructor)

  template <glm::qualifier Q = glm::defaultp>
  [[nodiscard]] constexpr operator glm::mat<3, 3, T, Q>() const noexcept;  // NOLINT(google-explicit-constructor)
#endif

  // Conversion constructors
  template <Scalar X0, Scalar Y0, Scalar Z0, Scalar X1, Scalar Y1, Scalar Z1, Scalar X2, Scalar Y2, Scalar Z2>
  constexpr mat(X0 x0, Y0 y0, Z0 z0, X1 x1, Y1 y1, Z1 z1, X2 x2, Y2 y2, Z2 z2) noexcept;

  template <typename V1, typename V2, typename V3>
  constexpr mat(const vec<3, V1>& v1, const vec<3, V2>& v2, const vec<3, V3>& v3) noexcept;

  template <typename U>
  constexpr explicit mat(const mat<3, U>& m) noexcept;

  template <typename U>
  constexpr explicit mat(const mat<4, U>& m) noexcept;

  // Unary arithmetic operators
  template <typename U>
  constexpr mat& operator=(const mat<3, U>& m) noexcept;

  template <Scalar U>
  constexpr mat& operator+=(U scalar) noexcept;
  template <typename U>
  constexpr mat& operator+=(const mat<3, U>& m) noexcept;

  template <Scalar U>
  constexpr mat& operator-=(U scalar) noexcept;
  template <typename U>
  constexpr mat& operator-=(const mat<3, U>& m) noexcept;

  template <Scalar U>
  constexpr mat& operator*=(U scalar) noexcept;
  template <typename U>
  constexpr mat& operator*=(const mat<3, U>& m) noexcept;

  template <Scalar U>
  constexpr mat& operator/=(U scalar) noexcept;
  template <typename U>
  constexpr mat& operator/=(const mat<3, U>& m) noexcept;

  // Increment and decrement operators
  constexpr mat& operator++() noexcept;
  constexpr mat& operator--() noexcept;
  [[nodiscard]] constexpr mat operator++(int) noexcept;
  [[nodiscard]] constexpr mat operator--(int) noexcept;
};

// Unary operators
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator+(mat<3, T> m) noexcept;
template <typename T>
[[nodiscard]] constexpr mat<3, T> operator-(const mat<3, T>& m) noexcept;

// Scalar binary arithmetic operators
template <typename T, Scalar U>
[[nodiscard]] constexpr mat<3, T> operator+(mat<3, T> m, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr mat<3, T> operator+(U scalar, mat<3, T> m) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr mat<3, T> operator-(mat<3, T> m, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr mat<3, T> operator-(U scalar, const mat<3, T>& m) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr mat<3, T> operator*(mat<3, T> m, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr mat<3, T> operator*(U scalar, mat<3, T> m) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr mat<3, T> operator/(mat<3, T> m, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr mat<3, T> operator/(U scalar, const mat<3, T>& m) noexcept;

// Matrix binary arithmetic operators
template <typename T, typename U>
[[nodiscard]] constexpr mat<3, T> operator+(mat<3, T> lhs, const mat<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr mat<3, T> operator-(mat<3, T> lhs, const mat<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr mat<3, T> operator*(const mat<3, T>& lhs, const mat<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr mat<3, T> operator/(mat<3, T> lhs, const mat<3, U>& rhs) noexcept;

// Matrix / vector operators
template <typename T, typename U>
[[nodiscard]] constexpr typename mat<3, T>::col_t operator*(const mat<3, T>& m, const vec<3, U>& v) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr typename mat<3, T>::row_t operator*(const vec<3, U>& v, const mat<3, T>& m) noexcept;

template <typename T, typename U>
[[nodiscard]] constexpr typename mat<3, T>::col_t operator/(const mat<3, T>& m, const vec<3, U>& v) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr typename mat<3, T>::row_t operator/(const vec<3, U>& v, const mat<3, T>& m) noexcept;

// Boolean operators
template <typename T, typename U>
[[nodiscard]] constexpr bool operator==(const mat<3, T>& m1, const mat<3, U>& m2) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr bool operator!=(const mat<3, T>& m1, const mat<3, U>& m2) noexcept;

}  // namespace mEn

#include "detail/mat3.inl"
