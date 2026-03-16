#pragma once

#include <mEn/fwd.hpp>

#if MEN_GLM
#include <glm/detail/qualifier.hpp>
#include <glm/vec4.hpp>
#endif

namespace mEn {

/// @brief 4-component vector with scalar type @p T.
///
/// Components are accessible via positional (@c x, @c y, @c z, @c w),
/// colour (@c r, @c g, @c b, @c a), or texture-coordinate (@c s, @c t, @c p, @c q) aliases.
/// @tparam T Component scalar type. Must satisfy @c Scalar.
template <typename T>
struct vec<4, T> {
  static_assert(Scalar<T>, "mEn::vec<4, T> requires an arithmetic scalar type");

  /// @brief Scalar type of the vector's components.
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

  /// @brief Returns the number of components (always 4).
  [[nodiscard]] static constexpr length_t length() noexcept { return 4; }

  /// @brief Returns a reference to the component at index @p i.
  /// @pre @p i < 4
  [[nodiscard]] constexpr T& operator[](length_t i) noexcept;
  /// @brief Returns a const reference to the component at index @p i.
  /// @pre @p i < 4
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
  /// @brief Constructs from explicit x, y, z, and w components.
  constexpr vec(T x, T y, T z, T w) noexcept;

#if MEN_GLM
  /// @brief Constructs from a GLM vec4 (implicit for transparent GLM interop).
  template <Scalar U, glm::qualifier Q>
  constexpr vec(const glm::vec<4, U, Q>& v) noexcept;  // NOLINT(google-explicit-constructor)

  /// @brief Converts to a GLM vec4 (implicit for transparent GLM interop).
  template <glm::qualifier Q = glm::defaultp>
  [[nodiscard]] constexpr operator glm::vec<4, T, Q>() const noexcept;  // NOLINT(google-explicit-constructor)
#endif

  /// @brief Constructs from independently-typed components, narrowing to @p T.
  template <Scalar X, Scalar Y, Scalar Z, Scalar W>
  constexpr vec(X x, Y y, Z z, W w) noexcept;

  /// @name Swizzled construction from sub-vectors and scalars
  /// @{
  /// @brief Constructs from @c vec<2,XY> and individual z and w scalars.
  template <typename XY, Scalar Z, Scalar W>
  constexpr vec(const vec<2, XY>& xy, Z z, W w) noexcept;
  /// @brief Constructs from scalar x, @c vec<2,YZ>, and scalar w.
  template <Scalar X, typename YZ, Scalar W>
  constexpr vec(X x, const vec<2, YZ>& yz, W w) noexcept;
  /// @brief Constructs from scalar x, scalar y, and @c vec<2,ZW>.
  template <Scalar X, Scalar Y, typename ZW>
  constexpr vec(X x, Y y, const vec<2, ZW>& zw) noexcept;

  /// @brief Constructs from scalar x and @c vec<3,YZW>.
  template <Scalar X, typename YZW>
  constexpr vec(X x, const vec<3, YZW>& yzw) noexcept;
  /// @brief Constructs from @c vec<3,XYZ> and scalar w.
  template <typename XYZ, Scalar W>
  constexpr vec(const vec<3, XYZ>& xyz, W w) noexcept;

  /// @brief Constructs from two @c vec<2> halves.
  template <typename XY, typename ZW>
  constexpr vec(const vec<2, XY>& xy, const vec<2, ZW>& zw) noexcept;
  /// @}

  /// @brief Narrowing conversion from @c vec<4,U>.
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

/// @name Unary operators
/// @{
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator+(vec<4, T> v) noexcept;
template <typename T>
[[nodiscard]] constexpr vec<4, T> operator-(const vec<4, T>& v) noexcept;
/// @}

/// @name Scalar binary arithmetic operators
/// @{
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
/// @}

/// @name Vector binary arithmetic operators
/// @{
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator+(vec<4, T> lhs, const vec<4, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator-(vec<4, T> lhs, const vec<4, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator*(vec<4, T> lhs, const vec<4, U>& rhs) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr vec<4, T> operator/(vec<4, T> lhs, const vec<4, U>& rhs) noexcept;
/// @}

/// @name Equality operators
/// Both operators promote to the common type of @p T and @p U before comparing.
/// When the common type is floating-point, comparison uses @c kEpsilon tolerance;
/// for integral types exact equality is used.
/// @{
template <typename T, typename U>
[[nodiscard]] constexpr bool operator==(const vec<4, T>& v1, const vec<4, U>& v2) noexcept;
template <typename T, typename U>
[[nodiscard]] constexpr bool operator!=(const vec<4, T>& v1, const vec<4, U>& v2) noexcept;
/// @}

}  // namespace mEn

#include "detail/vec4.inl"
