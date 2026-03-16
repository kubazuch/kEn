#pragma once

#include <mEn/fwd.hpp>

#if MEN_GLM
#include <glm/detail/qualifier.hpp>
#include <glm/vec3.hpp>
#endif

namespace mEn {

/// @brief 3-component vector with scalar type @p T.
///
/// Components are accessible via positional (@c x, @c y, @c z),
/// colour (@c r, @c g, @c b), or texture-coordinate (@c s, @c t, @c p) aliases.
/// @tparam T Component scalar type. Must satisfy @c Scalar.
template <typename T>
struct vec<3, T> {
  static_assert(Scalar<T>, "mEn::vec<3, T> requires an arithmetic scalar type");

  /// @brief Scalar type of the vector's components.
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

  /// @brief Returns the number of components (always 3).
  [[nodiscard]] static constexpr length_t length() noexcept { return 3; }

  /// @brief Returns a reference to the component at index @p i.
  /// @pre @p i < 3
  [[nodiscard]] constexpr T& operator[](length_t i) noexcept;
  /// @brief Returns a const reference to the component at index @p i.
  /// @pre @p i < 3
  [[nodiscard]] constexpr const T& operator[](length_t i) const noexcept;

  // Basic construction and assignment
  constexpr vec() noexcept                      = default;
  constexpr vec(const vec&) noexcept            = default;
  constexpr vec(vec&&) noexcept                 = default;
  constexpr vec& operator=(const vec&) noexcept = default;
  constexpr vec& operator=(vec&&) noexcept      = default;
  ~vec()                                        = default;

  /// @brief Constructs with all components set to @p scalar (splat).
  constexpr explicit vec(T scalar) noexcept;
  /// @brief Constructs from explicit x, y, and z components.
  constexpr vec(T x, T y, T z) noexcept;

#if MEN_GLM
  /// @brief Constructs from a GLM vec3 (implicit for transparent GLM interop).
  template <Scalar U, glm::qualifier Q>
  constexpr vec(const glm::vec<3, U, Q>& v) noexcept;  // NOLINT(google-explicit-constructor)

  /// @brief Converts to a GLM vec3 (implicit for transparent GLM interop).
  template <glm::qualifier Q = glm::defaultp>
  [[nodiscard]] constexpr operator glm::vec<3, T, Q>() const noexcept;  // NOLINT(google-explicit-constructor)
#endif

  /// @brief Constructs from independently-typed components, narrowing to @p T.
  template <Scalar X, Scalar Y, Scalar Z>
  constexpr vec(X x, Y y, Z z) noexcept;

  /// @brief Constructs from a @c vec<2,XY> and a scalar z.
  template <typename XY, Scalar Z>
  constexpr vec(const vec<2, XY>& xy, Z z) noexcept;
  /// @brief Constructs from a scalar x and a @c vec<2,YZ>.
  template <Scalar X, typename YZ>
  constexpr vec(X x, const vec<2, YZ>& yz) noexcept;

  /// @brief Narrowing conversion from @c vec<3,U>.
  template <typename U>
  constexpr explicit vec(const vec<3, U>& v) noexcept;

  /// @brief Constructs from the first three components of a @c vec<4,U>.
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

/// @name Unary operators
/// @{
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator+(vec<3, T> v) noexcept;
template <typename T>
[[nodiscard]] constexpr vec<3, T> operator-(const vec<3, T>& v) noexcept;
/// @}

/// @name Scalar binary arithmetic operators
/// @{
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
/// @}

/// @name Vector binary arithmetic operators
/// @{
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator+(vec<3, T> lhs, const vec<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator-(vec<3, T> lhs, const vec<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator*(vec<3, T> lhs, const vec<3, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<3, T> operator/(vec<3, T> lhs, const vec<3, U>& rhs) noexcept;
/// @}

/// @name Equality operators
/// Both operators promote to the common type of @p T and @p U before comparing.
/// When the common type is floating-point, comparison uses @c kEpsilon tolerance;
/// for integral types exact equality is used.
/// @{
template <typename T, typename U>
[[nodiscard]] constexpr bool operator==(const vec<3, T>& v1, const vec<3, U>& v2) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr bool operator!=(const vec<3, T>& v1, const vec<3, U>& v2) noexcept;
/// @}

}  // namespace mEn

#include "detail/vec3.inl"
