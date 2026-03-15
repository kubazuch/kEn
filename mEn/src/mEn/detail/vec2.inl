#include <mEn/constants.hpp>

namespace mEn {
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)

// Components
template <typename T>
MEN_FORCE_INLINE constexpr T& vec<2, T>::operator[](length_t i) noexcept {
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(i < length());
  }
  return (i == 0) ? x : y;
}

template <typename T>
MEN_FORCE_INLINE constexpr const T& vec<2, T>::operator[](length_t i) const noexcept {
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(i < length());
  }
  return (i == 0) ? x : y;
}

// Explicit constructors
template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T>::vec(T scalar) noexcept : x(scalar), y(scalar) {}

template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T>::vec(T x, T y) noexcept : x(x), y(y) {}

#if MEN_GLM
template <typename T>
template <Scalar U, glm::qualifier Q>
MEN_FORCE_INLINE constexpr vec<2, T>::vec(const glm::vec<2, U, Q>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

template <typename T>
template <glm::qualifier Q>
MEN_FORCE_INLINE constexpr vec<2, T>::operator glm::vec<2, T, Q>() const noexcept {
  return glm::vec<2, T, Q>{x, y};
}
#endif

// Conversion constructors
template <typename T>
template <Scalar X, Scalar Y>
MEN_FORCE_INLINE constexpr vec<2, T>::vec(X x, Y y) noexcept : x(static_cast<T>(x)), y(static_cast<T>(y)) {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<2, T>::vec(const vec<2, U>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<2, T>::vec(const vec<3, U>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<2, T>::vec(const vec<4, U>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

// Unary arithmetic operators
template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator+=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x += s;
  y += s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator-=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x -= s;
  y -= s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator*=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x *= s;
  y *= s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator/=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x /= s;
  y /= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator=(const vec<2, U>& v) noexcept {
  x = static_cast<T>(v.x);
  y = static_cast<T>(v.y);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator+=(const vec<2, U>& v) noexcept {
  x += static_cast<T>(v.x);
  y += static_cast<T>(v.y);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator-=(const vec<2, U>& v) noexcept {
  x -= static_cast<T>(v.x);
  y -= static_cast<T>(v.y);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator*=(const vec<2, U>& v) noexcept {
  x *= static_cast<T>(v.x);
  y *= static_cast<T>(v.y);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator/=(const vec<2, U>& v) noexcept {
  x /= static_cast<T>(v.x);
  y /= static_cast<T>(v.y);
  return *this;
}

// Increment and decrement operators
template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator++() noexcept {
  ++x;
  ++y;
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T>& vec<2, T>::operator--() noexcept {
  --x;
  --y;
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T> vec<2, T>::operator++(int) noexcept {
  vec result(*this);
  ++(*this);
  return result;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T> vec<2, T>::operator--(int) noexcept {
  vec result(*this);
  --(*this);
  return result;
}

// Unary operators
template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T> operator+(vec<2, T> v) noexcept {
  return v;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T> operator-(const vec<2, T>& v) noexcept {
  return vec<2, T>{static_cast<T>(-v.x), static_cast<T>(-v.y)};
}

// Scalar binary arithmetic operators
template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T> operator+(vec<2, T> v, U scalar) noexcept {
  v += scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T> operator+(U scalar, vec<2, T> v) noexcept {
  v += scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T> operator-(vec<2, T> v, U scalar) noexcept {
  v -= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T> operator-(U scalar, const vec<2, T>& v) noexcept {
  vec<2, T> out{static_cast<T>(scalar)};
  out -= v;
  return out;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T> operator*(vec<2, T> v, U scalar) noexcept {
  v *= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T> operator*(U scalar, vec<2, T> v) noexcept {
  v *= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T> operator/(vec<2, T> v, U scalar) noexcept {
  v /= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<2, T> operator/(U scalar, const vec<2, T>& v) noexcept {
  vec<2, T> out{static_cast<T>(scalar)};
  out /= v;
  return out;
}

// Vector binary arithmetic operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<2, T> operator+(vec<2, T> lhs, const vec<2, U>& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<2, T> operator-(vec<2, T> lhs, const vec<2, U>& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<2, T> operator*(vec<2, T> lhs, const vec<2, U>& rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<2, T> operator/(vec<2, T> lhs, const vec<2, U>& rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

// Boolean operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator==(const vec<2, T>& v1, const vec<2, U>& v2) noexcept {
  using C    = ::std::common_type_t<T, U>;
  const C ax = static_cast<C>(v1.x);
  const C ay = static_cast<C>(v1.y);
  const C bx = static_cast<C>(v2.x);
  const C by = static_cast<C>(v2.y);

  if constexpr (Floating<C>) {
    const C eps      = kEpsilon<C>;
    const auto error = [](C a, C b) noexcept -> C { return (a > b) ? (a - b) : (b - a); };
    return error(ax, bx) <= eps && error(ay, by) <= eps;
  } else {
    return ax == bx && ay == by;
  }
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator!=(const vec<2, T>& v1, const vec<2, U>& v2) noexcept {
  return !(v1 == v2);
}

// NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}  // namespace mEn