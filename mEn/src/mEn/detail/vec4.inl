#include <mEn/constants.hpp>

namespace mEn {
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)

// Components
template <typename T>
MEN_FORCE_INLINE constexpr T& vec<4, T>::operator[](length_t i) noexcept {
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
    case 3:
      return w;
  }
}

template <typename T>
MEN_FORCE_INLINE constexpr const T& vec<4, T>::operator[](length_t i) const noexcept {
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
    case 3:
      return w;
  }
}

// Explicit constructors
template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}

template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}

#if MEN_GLM
template <typename T>
template <Scalar U, glm::qualifier Q>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(const glm::vec<4, U, Q>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {}

template <typename T>
template <glm::qualifier Q>
MEN_FORCE_INLINE constexpr vec<4, T>::operator glm::vec<4, T, Q>() const noexcept {
  return glm::vec<4, T, Q>{x, y, z, w};
}
#endif

// Conversion constructors
template <typename T>
template <Scalar X, Scalar Y, Scalar Z, Scalar W>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(X x, Y y, Z z, W w) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

template <typename T>
template <typename XY, Scalar Z, Scalar W>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(const vec<2, XY>& xy, Z z, W w) noexcept
    : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

template <typename T>
template <Scalar X, typename YZ, Scalar W>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(X x, const vec<2, YZ>& yz, W w) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w)) {}

template <typename T>
template <Scalar X, Scalar Y, typename ZW>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(X x, Y y, const vec<2, ZW>& zw) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}

template <typename T>
template <Scalar X, typename YZW>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(X x, const vec<3, YZW>& yzw) noexcept
    : x(static_cast<T>(x)), y(static_cast<T>(yzw.x)), z(static_cast<T>(yzw.y)), w(static_cast<T>(yzw.z)) {}

template <typename T>
template <typename XYZ, Scalar W>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(const vec<3, XYZ>& xyz, W w) noexcept
    : x(static_cast<T>(xyz.x)), y(static_cast<T>(xyz.y)), z(static_cast<T>(xyz.z)), w(static_cast<T>(w)) {}

template <typename T>
template <typename XY, typename ZW>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(const vec<2, XY>& xy, const vec<2, ZW>& zw) noexcept
    : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<4, T>::vec(const vec<4, U>& v) noexcept
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {}

// Unary arithmetic operators
template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator+=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x += s;
  y += s;
  z += s;
  w += s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator-=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x -= s;
  y -= s;
  z -= s;
  w -= s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator*=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x *= s;
  y *= s;
  z *= s;
  w *= s;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator/=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x /= s;
  y /= s;
  z /= s;
  w /= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator=(const vec<4, U>& v) noexcept {
  x = static_cast<T>(v.x);
  y = static_cast<T>(v.y);
  z = static_cast<T>(v.z);
  w = static_cast<T>(v.w);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator+=(const vec<4, U>& v) noexcept {
  x += static_cast<T>(v.x);
  y += static_cast<T>(v.y);
  z += static_cast<T>(v.z);
  w += static_cast<T>(v.w);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator-=(const vec<4, U>& v) noexcept {
  x -= static_cast<T>(v.x);
  y -= static_cast<T>(v.y);
  z -= static_cast<T>(v.z);
  w -= static_cast<T>(v.w);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator*=(const vec<4, U>& v) noexcept {
  x *= static_cast<T>(v.x);
  y *= static_cast<T>(v.y);
  z *= static_cast<T>(v.z);
  w *= static_cast<T>(v.w);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator/=(const vec<4, U>& v) noexcept {
  x /= static_cast<T>(v.x);
  y /= static_cast<T>(v.y);
  z /= static_cast<T>(v.z);
  w /= static_cast<T>(v.w);
  return *this;
}

// Increment and decrement operators
template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator++() noexcept {
  ++x;
  ++y;
  ++z;
  ++w;
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T>& vec<4, T>::operator--() noexcept {
  --x;
  --y;
  --z;
  --w;
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T> vec<4, T>::operator++(int) noexcept {
  vec result(*this);
  ++(*this);
  return result;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T> vec<4, T>::operator--(int) noexcept {
  vec result(*this);
  --(*this);
  return result;
}

// Unary operators
template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T> operator+(vec<4, T> v) noexcept {
  return v;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T> operator-(const vec<4, T>& v) noexcept {
  return vec<4, T>{static_cast<T>(-v.x), static_cast<T>(-v.y), static_cast<T>(-v.z), static_cast<T>(-v.w)};
}

// Scalar binary arithmetic operators
template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T> operator+(vec<4, T> v, U scalar) noexcept {
  v += scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T> operator+(U scalar, vec<4, T> v) noexcept {
  v += scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T> operator-(vec<4, T> v, U scalar) noexcept {
  v -= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T> operator-(U scalar, const vec<4, T>& v) noexcept {
  vec<4, T> out{static_cast<T>(scalar)};
  out -= v;
  return out;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T> operator*(vec<4, T> v, U scalar) noexcept {
  v *= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T> operator*(U scalar, vec<4, T> v) noexcept {
  v *= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T> operator/(vec<4, T> v, U scalar) noexcept {
  v /= scalar;
  return v;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr vec<4, T> operator/(U scalar, const vec<4, T>& v) noexcept {
  vec<4, T> out{static_cast<T>(scalar)};
  out /= v;
  return out;
}

// Vector binary arithmetic operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<4, T> operator+(vec<4, T> lhs, const vec<4, U>& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<4, T> operator-(vec<4, T> lhs, const vec<4, U>& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<4, T> operator*(vec<4, T> lhs, const vec<4, U>& rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<4, T> operator/(vec<4, T> lhs, const vec<4, U>& rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

// Boolean operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator==(const vec<4, T>& v1, const vec<4, U>& v2) noexcept {
  using C    = ::std::common_type_t<T, U>;
  const C ax = static_cast<C>(v1.x);
  const C ay = static_cast<C>(v1.y);
  const C az = static_cast<C>(v1.z);
  const C aw = static_cast<C>(v1.w);
  const C bx = static_cast<C>(v2.x);
  const C by = static_cast<C>(v2.y);
  const C bz = static_cast<C>(v2.z);
  const C bw = static_cast<C>(v2.w);

  if constexpr (Floating<C>) {
    const C eps      = kEpsilon<C>;
    const auto error = [](C a, C b) noexcept -> C { return (a > b) ? (a - b) : (b - a); };
    return error(ax, bx) <= eps && error(ay, by) <= eps && error(az, bz) <= eps && error(aw, bw) <= eps;
  } else {
    return ax == bx && ay == by && az == bz && aw == bw;
  }
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator!=(const vec<4, T>& v1, const vec<4, U>& v2) noexcept {
  return !(v1 == v2);
}

// NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}  // namespace mEn
