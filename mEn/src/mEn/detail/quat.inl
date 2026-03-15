#include <mEn/functions/exponential.hpp>
#include <mEn/functions/geometric.hpp>
#include <mEn/functions/trigonometric.hpp>
#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

namespace mEn {

namespace detail {

template <typename T>
[[nodiscard]] MEN_FORCE_INLINE constexpr qua<T> inverse(const qua<T>& q) noexcept {
  const T dot = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
  return qua<T>(q.w / dot, -q.x / dot, -q.y / dot, -q.z / dot);
}

}  // namespace detail

// Components
template <typename T>
MEN_FORCE_INLINE constexpr T& qua<T>::operator[](length_t i) noexcept {
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
MEN_FORCE_INLINE constexpr const T& qua<T>::operator[](length_t i) const noexcept {
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
MEN_FORCE_INLINE constexpr qua<T>::qua(T s, const vec<3, T>& v) noexcept : x(v.x), y(v.y), z(v.z), w(s) {}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T>::qua(T _w, T _x, T _y, T _z) noexcept : x(_x), y(_y), z(_z), w(_w) {}

#if MEN_GLM
template <typename T>
template <Scalar U, glm::qualifier Q>
MEN_FORCE_INLINE constexpr qua<T>::qua(const glm::qua<U, Q>& q) noexcept
    : x(static_cast<T>(q.x)), y(static_cast<T>(q.y)), z(static_cast<T>(q.z)), w(static_cast<T>(q.w)) {}

template <typename T>
template <glm::qualifier Q>
MEN_FORCE_INLINE constexpr qua<T>::operator glm::qua<T, Q>() const noexcept {
  return glm::qua<T, Q>{w, x, y, z};
}
#endif

// Conversion constructors
template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr qua<T>::qua(const qua<U>& q) noexcept
    : x(static_cast<T>(q.x)), y(static_cast<T>(q.y)), z(static_cast<T>(q.z)), w(static_cast<T>(q.w)) {}

template <typename T>
MEN_FORCE_INLINE qua<T>::operator mat<3, T>() const noexcept {
  const T qxx = x * x;
  const T qyy = y * y;
  const T qzz = z * z;
  const T qxz = x * z;
  const T qxy = x * y;
  const T qyz = y * z;
  const T qwx = w * x;
  const T qwy = w * y;
  const T qwz = w * z;

  return mat<3, T>(vec<3, T>(T{1} - T{2} * (qyy + qzz), T{2} * (qxy + qwz), T{2} * (qxz - qwy)),
                   vec<3, T>(T{2} * (qxy - qwz), T{1} - T{2} * (qxx + qzz), T{2} * (qyz + qwx)),
                   vec<3, T>(T{2} * (qxz + qwy), T{2} * (qyz - qwx), T{1} - T{2} * (qxx + qyy)));
}

template <typename T>
MEN_FORCE_INLINE qua<T>::operator mat<4, T>() const noexcept {
  return mat<4, T>(static_cast<mat<3, T>>(*this));
}

template <typename T>
MEN_FORCE_INLINE qua<T>::qua(const vec<3, T>& u, const vec<3, T>& v) noexcept {
  T norm  = sqrt(dot(u, u) * dot(v, v));
  T angle = norm + dot(u, v);
  vec<3, T> axis;

  if (angle < static_cast<T>(1e-6) * norm) {
    angle = T{0};
    axis  = abs(u.x) > abs(u.z) ? vec<3, T>(-u.y, u.x, T{0}) : vec<3, T>(T{0}, -u.z, u.y);
  } else {
    axis = cross(u, v);
  }

  vec<4, T> quat = normalize(vec<4, T>(axis.x, axis.y, axis.z, angle));

  *this = qua<T>(quat.w, quat.x, quat.y, quat.z);
}

template <typename T>
MEN_FORCE_INLINE qua<T>::qua(const vec<3, T>& eulerAngles) noexcept {
  const vec<3, T> c = cos(eulerAngles * T{0.5});
  const vec<3, T> s = sin(eulerAngles * T{0.5});

  this->w = c.x * c.y * c.z + s.x * s.y * s.z;
  this->x = s.x * c.y * c.z - c.x * s.y * s.z;
  this->y = c.x * s.y * c.z + s.x * c.y * s.z;
  this->z = c.x * c.y * s.z - s.x * s.y * c.z;
}

template <typename T>
MEN_FORCE_INLINE qua<T>::qua(const mat<3, T>& m) noexcept {
  // Each diagonal combination equals 4c^2 - 1 for the corresponding component c.
  const T trace  = m[0].x + m[1].y + m[2].z;  // 4w^2 - 1
  const T diag_x = m[0].x - m[1].y - m[2].z;  // 4x^2 - 1
  const T diag_y = m[1].y - m[0].x - m[2].z;  // 4y^2 - 1
  const T diag_z = m[2].z - m[0].x - m[1].y;  // 4z^2 - 1

  // Pick the largest to avoid near-zero division when computing the others.
  int dominant_idx = 0;
  T biggest        = trace;
  if (diag_x > biggest) {
    biggest      = diag_x;
    dominant_idx = 1;
  }
  if (diag_y > biggest) {
    biggest      = diag_y;
    dominant_idx = 2;
  }
  if (diag_z > biggest) {
    biggest      = diag_z;
    dominant_idx = 3;
  }

  const T dominant = sqrt(biggest + T{1}) * T{0.5};
  const T scale    = T{0.25} / dominant;

  switch (dominant_idx) {
    default:
    case 0:
      *this = qua<T>(dominant, (m[1].z - m[2].y) * scale, (m[2].x - m[0].z) * scale, (m[0].y - m[1].x) * scale);
      break;
    case 1:
      *this = qua<T>((m[1].z - m[2].y) * scale, dominant, (m[0].y + m[1].x) * scale, (m[2].x + m[0].z) * scale);
      break;
    case 2:
      *this = qua<T>((m[2].x - m[0].z) * scale, (m[0].y + m[1].x) * scale, dominant, (m[1].z + m[2].y) * scale);
      break;
    case 3:
      *this = qua<T>((m[0].y - m[1].x) * scale, (m[2].x + m[0].z) * scale, (m[1].z + m[2].y) * scale, dominant);
      break;
  }
}

template <typename T>
MEN_FORCE_INLINE qua<T>::qua(const mat<4, T>& m) noexcept {
  *this = qua<T>(mat<3, T>(m));
}

// Unary arithmetic operators
template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr qua<T>& qua<T>::operator=(const qua<U>& q) noexcept {
  x = static_cast<T>(q.x);
  y = static_cast<T>(q.y);
  z = static_cast<T>(q.z);
  w = static_cast<T>(q.w);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr qua<T>& qua<T>::operator+=(const qua<U>& q) noexcept {
  x += static_cast<T>(q.x);
  y += static_cast<T>(q.y);
  z += static_cast<T>(q.z);
  w += static_cast<T>(q.w);
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr qua<T>& qua<T>::operator-=(const qua<U>& q) noexcept {
  x -= static_cast<T>(q.x);
  y -= static_cast<T>(q.y);
  z -= static_cast<T>(q.z);
  w -= static_cast<T>(q.w);
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr qua<T>& qua<T>::operator*=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x *= s;
  y *= s;
  z *= s;
  w *= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr qua<T>& qua<T>::operator*=(const qua<U>& q) noexcept {
  const qua<T> p(*this);
  const T qw = static_cast<T>(q.w);
  const T qx = static_cast<T>(q.x);
  const T qy = static_cast<T>(q.y);
  const T qz = static_cast<T>(q.z);
  this->w    = p.w * qw - p.x * qx - p.y * qy - p.z * qz;
  this->x    = p.w * qx + p.x * qw + p.y * qz - p.z * qy;
  this->y    = p.w * qy + p.y * qw + p.z * qx - p.x * qz;
  this->z    = p.w * qz + p.z * qw + p.x * qy - p.y * qx;
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr qua<T>& qua<T>::operator/=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  x /= s;
  y /= s;
  z /= s;
  w /= s;
  return *this;
}

// Unary operators
template <typename T>
MEN_FORCE_INLINE constexpr qua<T> operator+(qua<T> q) noexcept {
  return q;
}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> operator-(const qua<T>& q) noexcept {
  return qua<T>(-q.w, -q.x, -q.y, -q.z);
}

// Scalar binary arithmetic operators
template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr qua<T> operator*(qua<T> q, U scalar) noexcept {
  q *= scalar;
  return q;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr qua<T> operator*(U scalar, qua<T> q) noexcept {
  q *= scalar;
  return q;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr qua<T> operator/(qua<T> q, U scalar) noexcept {
  q /= scalar;
  return q;
}

// Vector binary arithmetic operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<3, T> operator*(const qua<T>& q, const vec<3, U>& v) noexcept {
  const vec<3, T> vt(v);
  const vec<3, T> axis(q.x, q.y, q.z);
  const vec<3, T> qv(cross(axis, vt));
  const vec<3, T> qqv(cross(axis, qv));
  return vt + T{2} * (q.w * qv + qqv);
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<3, T> operator*(const vec<3, U>& v, const qua<T>& q) noexcept {
  return detail::inverse(q) * v;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<4, T> operator*(const qua<T>& q, const vec<4, U>& v) noexcept {
  return vec<4, T>(q * vec<3, T>(v), static_cast<T>(v.w));
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr vec<4, T> operator*(const vec<4, U>& v, const qua<T>& q) noexcept {
  return detail::inverse(q) * v;
}

// Quaternion binary arithmetic operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr qua<T> operator+(qua<T> lhs, const qua<U>& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr qua<T> operator-(qua<T> lhs, const qua<U>& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr qua<T> operator*(qua<T> lhs, const qua<U>& rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

// Boolean operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator==(const qua<T>& q1, const qua<U>& q2) noexcept {
  const vec<4, T> v1(q1.x, q1.y, q1.z, q1.w);
  const vec<4, U> v2(q2.x, q2.y, q2.z, q2.w);
  return v1 == v2;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator!=(const qua<T>& q1, const qua<U>& q2) noexcept {
  return !(q1 == q2);
}

}  // namespace mEn
