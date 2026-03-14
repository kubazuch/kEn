#include <mEn/constants.hpp>

namespace mEn {  // NOLINT

// Components
template <typename T>
MEN_FORCE_INLINE constexpr T& vec<3, T>::operator[](length_t i) noexcept {
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(i < length());
  }
  switch (i) {
    default:
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
  }
}

template <typename T>
MEN_FORCE_INLINE constexpr const T& vec<3, T>::operator[](length_t i) const noexcept {
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(i < length());
  }
  switch (i) {
    default:
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
  }
}

// Explicit constructors
template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T>::vec(T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}

template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T>::vec(T x, T y, T z) noexcept : x(x), y(y), z(z) {}

#if MEN_GLM
template <typename T>
template <Scalar U, glm::qualifier Q>
MEN_FORCE_INLINE constexpr vec<3, T>::vec(const glm::vec<3, U, Q>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}

template <typename T>
template <glm::qualifier Q>
MEN_FORCE_INLINE constexpr vec<3, T>::operator glm::vec<3, T, Q>() const noexcept {
  return glm::vec<3, T, Q>{x, y, z};
}
#endif

// Conversion constructors
template <typename T>
template <Scalar X, Scalar Y, Scalar Z>
MEN_FORCE_INLINE constexpr vec<3, T>::vec(X x, Y y, Z z) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}

template <typename T>
template <typename XY, Scalar Z>
MEN_FORCE_INLINE constexpr vec<3, T>::vec(const vec<2, XY>& xy, Z z) noexcept
    : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z)) {}

template <typename T>
template <Scalar X, typename YZ>
MEN_FORCE_INLINE constexpr vec<3, T>::vec(X x, const vec<2, YZ>& yz) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)) {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<3, T>::vec(const vec<3, U>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<3, T>::vec(const vec<4, U>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}

// Unary arithmetic operators
template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator+=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x += s;
  y += s;
  z += s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator-=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x -= s;
  y -= s;
  z -= s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator*=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x *= s;
  y *= s;
  z *= s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator/=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x /= s;
  y /= s;
  z /= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator=(const vec<3, U>& v) noexcept {
  x = static_cast<T>(v.x);
  y = static_cast<T>(v.y);
  z = static_cast<T>(v.z);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator+=(const vec<3, U>& v) noexcept {
  x += static_cast<T>(v.x);
  y += static_cast<T>(v.y);
  z += static_cast<T>(v.z);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator-=(const vec<3, U>& v) noexcept {
  x -= static_cast<T>(v.x);
  y -= static_cast<T>(v.y);
  z -= static_cast<T>(v.z);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator*=(const vec<3, U>& v) noexcept {
  x *= static_cast<T>(v.x);
  y *= static_cast<T>(v.y);
  z *= static_cast<T>(v.z);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator/=(const vec<3, U>& v) noexcept {
  x /= static_cast<T>(v.x);
  y /= static_cast<T>(v.y);
  z /= static_cast<T>(v.z);
  return *this;
}

// Increment and decrement operators
template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator++() noexcept {
  ++x;
  ++y;
  ++z;
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T>& vec<3, T>::operator--() noexcept {
  --x;
  --y;
  --z;
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T> vec<3, T>::operator++(int) noexcept {
  vec result(*this);
  ++(*this);
  return result;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T> vec<3, T>::operator--(int) noexcept {
  vec result(*this);
  --(*this);
  return result;
}

// Unary operators
template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T> operator+(vec<3, T> v) noexcept {
  return v;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T> operator-(const vec<3, T>& v) noexcept {
  return vec<3, T>{static_cast<T>(-v.x), static_cast<T>(-v.y), static_cast<T>(-v.z)};
}

// Scalar binary arithmetic operators
template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T> operator+(vec<3, T> v, U scalar) noexcept {
  v += scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T> operator+(U scalar, vec<3, T> v) noexcept {
  v += scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T> operator-(vec<3, T> v, U scalar) noexcept {
  v -= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T> operator-(U scalar, const vec<3, T>& v) noexcept {
  vec<3, T> out{static_cast<T>(scalar)};
  out -= v;
  return out;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T> operator*(vec<3, T> v, U scalar) noexcept {
  v *= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T> operator*(U scalar, vec<3, T> v) noexcept {
  v *= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T> operator/(vec<3, T> v, U scalar) noexcept {
  v /= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<3, T> operator/(U scalar, const vec<3, T>& v) noexcept {
  vec<3, T> out{static_cast<T>(scalar)};
  out /= v;
  return out;
}

// Vector binary arithmetic operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<3, T> operator+(vec<3, T> lhs, const vec<3, U>& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<3, T> operator-(vec<3, T> lhs, const vec<3, U>& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<3, T> operator*(vec<3, T> lhs, const vec<3, U>& rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<3, T> operator/(vec<3, T> lhs, const vec<3, U>& rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

// Boolean operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator==(const vec<3, T>& v1, const vec<3, U>& v2) noexcept {
  using C    = ::std::common_type_t<T, U>;
  const C ax = static_cast<C>(v1.x);
  const C ay = static_cast<C>(v1.y);
  const C az = static_cast<C>(v1.z);
  const C bx = static_cast<C>(v2.x);
  const C by = static_cast<C>(v2.y);
  const C bz = static_cast<C>(v2.z);

  if constexpr (Floating<C>) {
    const C eps      = kEpsilon<C>;
    const auto error = [](C a, C b) noexcept -> C { return (a > b) ? (a - b) : (b - a); };
    return error(ax, bx) <= eps && error(ay, by) <= eps && error(az, bz) <= eps;
  } else {
    return ax == bx && ay == by && az == bz;
  }
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator!=(const vec<3, T>& v1, const vec<3, U>& v2) noexcept {
  return !(v1 == v2);
}

}  // namespace mEn
