#pragma once

#include <mEn/fwd.hpp>

#if MEN_GLM
#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_quat.hpp>
#endif

/// @file
/// @ingroup men

namespace mEn {

/// @brief Quaternion with scalar type @p T, representing a rotation in 3D.
///
/// Stored as four components @c {x, y, z, w} where @c {x, y, z} is the
/// vector (imaginary) part and @c w is the scalar (real) part.
/// A unit quaternion satisfies @f$ x^2 + y^2 + z^2 + w^2 = 1 @f$.
/// @tparam T Component scalar type. Must satisfy @c Scalar.
template <typename T>
struct qua {
  static_assert(Scalar<T>, "mEn::qua<T> requires an arithmetic scalar type");

  /// @brief Scalar type of the quaternion's components.
  using value_t = T;

#if MEN_GLM
  using glm_type = glm::qua<T, glm::defaultp>;
#endif

  T x, y, z, w;

  /// @brief Returns the number of components (always 4).
  [[nodiscard]] static constexpr length_t length() noexcept { return 4; }

  /// @brief Returns a reference to the component at index @p i (x=0, y=1, z=2, w=3).
  /// @pre @p i < 4
  constexpr T& operator[](length_t i) noexcept;
  /// @brief Returns a const reference to the component at index @p i (x=0, y=1, z=2, w=3).
  /// @pre @p i < 4
  constexpr const T& operator[](length_t i) const noexcept;

  // Basic construction and assignment
  constexpr qua() noexcept                      = default;
  constexpr qua(const qua& v) noexcept          = default;
  constexpr qua(qua&&) noexcept                 = default;
  constexpr qua& operator=(const qua&) noexcept = default;
  constexpr qua& operator=(qua&&) noexcept      = default;
  ~qua()                                        = default;

  /// @brief Constructs from a scalar (real) part @p s and a vector (imaginary) part @p v.
  constexpr qua(T s, const vec<3, T>& v) noexcept;
  /// @brief Constructs from individual components.
  /// @note Parameters are ordered @c (w, x, y, z) but stored as @c {x, y, z, w}.
  constexpr qua(T w, T x, T y, T z) noexcept;

#if MEN_GLM
  /// @brief Constructs from a GLM quaternion (implicit for transparent GLM interop).
  template <Scalar U, glm::qualifier Q>
  constexpr qua(const glm::qua<U, Q>& q) noexcept;  // NOLINT(google-explicit-constructor)

  /// @brief Converts to a GLM quaternion (implicit for transparent GLM interop).
  template <glm::qualifier Q = glm::defaultp>
  [[nodiscard]] constexpr operator glm::qua<T, Q>() const noexcept;  // NOLINT(google-explicit-constructor)
#endif

  /// @brief Narrowing conversion from @c qua&lt;U&gt;.
  template <typename U>
  constexpr explicit qua(const qua<U>& q) noexcept;

  /// @brief Converts to the equivalent 3x3 rotation matrix.
  [[nodiscard]] explicit operator mat<3, T>() const noexcept;
  /// @brief Converts to the equivalent 4x4 rotation matrix.
  [[nodiscard]] explicit operator mat<4, T>() const noexcept;

  /// @brief Constructs the shortest-arc rotation that maps unit vector @p u to unit vector @p v.
  qua(const vec<3, T>& u, const vec<3, T>& v) noexcept;

  /// @brief Constructs from Euler angles (roll, pitch, yaw) in radians.
  explicit qua(const vec<3, T>& eulerAngles) noexcept;
  /// @brief Constructs from a 3x3 rotation matrix.
  explicit qua(const mat<3, T>& m) noexcept;
  /// @brief Constructs from the upper-left 3x3 rotation block of a 4x4 matrix.
  explicit qua(const mat<4, T>& m) noexcept;

  // Unary arithmetic operators
  template <typename U>
  constexpr qua& operator=(const qua<U>& q) noexcept;

  template <typename U>
  constexpr qua& operator+=(const qua<U>& q) noexcept;

  template <typename U>
  constexpr qua& operator-=(const qua<U>& q) noexcept;

  /// @brief Multiplies by scalar @p scalar component-wise.
  template <Scalar U>
  constexpr qua& operator*=(U scalar) noexcept;
  /// @brief Multiplies by quaternion @p q (Hamilton product).
  template <typename U>
  constexpr qua& operator*=(const qua<U>& q) noexcept;

  template <Scalar U>
  constexpr qua& operator/=(U scalar) noexcept;
};

/// @name Unary operators
/// @{
template <typename T>
[[nodiscard]] constexpr qua<T> operator+(qua<T> q) noexcept;
template <typename T>
[[nodiscard]] constexpr qua<T> operator-(const qua<T>& q) noexcept;
/// @}

/// @name Scalar binary arithmetic operators
/// @{
template <typename T, Scalar U>
[[nodiscard]] constexpr qua<T> operator*(qua<T> q, U scalar) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr qua<T> operator*(U scalar, qua<T> q) noexcept;
template <typename T, Scalar U>
[[nodiscard]] constexpr qua<T> operator/(qua<T> q, U scalar) noexcept;
/// @}

/// @name Quaternion / vector operators
/// Rotates the vector by the quaternion's rotation.
/// Both @c vec<3> and @c vec<4> variants are provided.
/// @{
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator*(const qua<T>& q, const vec<3, U>& v) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator*(const vec<3, U>& v, const qua<T>& q) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator*(const qua<T>& q, const vec<4, U>& v) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator*(const vec<4, U>& v, const qua<T>& q) noexcept;
/// @}

/// @name Quaternion binary arithmetic operators
/// Note: @c operator* performs the Hamilton product (composition of rotations).
/// @{
template <typename T, typename U>
[[nodiscard]] constexpr qua<T> operator+(qua<T> lhs, const qua<U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr qua<T> operator-(qua<T> lhs, const qua<U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr qua<T> operator*(qua<T> lhs, const qua<U>& rhs) noexcept;
/// @}

/// @name Equality operators
/// Component-wise comparison. Floating-point components use @c kEpsilon tolerance.
/// @{
template <typename T, typename U>
[[nodiscard]] constexpr bool operator==(const qua<T>& q1, const qua<U>& q2) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr bool operator!=(const qua<T>& q1, const qua<U>& q2) noexcept;
/// @}

}  // namespace mEn

#include "detail/quat.inl"
