#pragma once
#include "functions/quat_functions.hpp"
#include "functions/vec_functions.hpp"
#include "quat.hpp"

namespace mEn {

// Components
inline constexpr float& Quat::operator[](int i) {
  assert(i >= 0 && i < length());
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

inline constexpr const float& Quat::operator[](int i) const {
  assert(i >= 0 && i < length());
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

inline constexpr Quat::Quat(float s, const Vec3& v) : x(v.x), y(v.y), z(v.z), w(s) {}
inline constexpr Quat::Quat(float _w, float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(_w) {}

inline Quat::Quat(const Vec3& u, const Vec3& v) {
  float norm  = sqrt(dot(u, u) * dot(v, v));
  float angle = norm + dot(u, v);
  Vec3 axis;

  if (angle < 1e-5F * norm) {
    angle = 0.0F;
    axis  = abs(u.x) > abs(u.z) ? Vec3(-u.y, u.x, 0.0F) : Vec3(0.0F, -u.z, u.y);
  } else {
    axis = cross(u, v);
  }

  *this = normalize(Quat(angle, axis.x, axis.y, axis.z));
}

inline Quat::Quat(const Vec3& eulerAngles) {
  Vec3 c = {cos(eulerAngles.x * 0.5F), cos(eulerAngles.y * 0.5F), cos(eulerAngles.z * 0.5F)};
  Vec3 s = {sin(eulerAngles.x * 0.5F), sin(eulerAngles.y * 0.5F), sin(eulerAngles.z * 0.5F)};

  this->w = c.x * c.y * c.z + s.x * s.y * s.z;
  this->x = s.x * c.y * c.z - c.x * s.y * s.z;
  this->y = c.x * s.y * c.z + s.x * c.y * s.z;
  this->z = c.x * c.y * s.z - s.x * s.y * c.z;
}

inline Quat::Quat(const Mat3& m) { *this = quat_cast(m); }

inline Quat::Quat(const Mat4& m) { *this = quat_cast(m); }

inline Quat::operator Mat3() const { return mat3_cast(*this); }

inline Quat::operator Mat4() const { return mat4_cast(*this); }

// Unary arithmetic operators

inline constexpr Quat& Quat::operator+=(const Quat& q) {
  this->x += q.x;
  this->y += q.y;
  this->z += q.z;
  this->w += q.w;
  return *this;
}

inline constexpr Quat& Quat::operator-=(const Quat& q) {
  this->x -= q.x;
  this->y -= q.y;
  this->z -= q.z;
  this->w -= q.w;
  return *this;
}

template <typename U>
inline constexpr Quat& Quat::operator*=(U scalar) {
  this->x *= static_cast<float>(scalar);
  this->y *= static_cast<float>(scalar);
  this->z *= static_cast<float>(scalar);
  this->w *= static_cast<float>(scalar);
  return *this;
}

constexpr Quat& Quat::operator*=(const Quat& q) {
  const Quat p(*this);
  this->w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
  this->x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
  this->y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
  this->z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
  return *this;
}

template <typename U>
inline constexpr Quat& Quat::operator/=(U scalar) {
  this->x *= static_cast<float>(scalar);
  this->y *= static_cast<float>(scalar);
  this->z *= static_cast<float>(scalar);
  this->w *= static_cast<float>(scalar);
  return *this;
}

// Unary operators

inline constexpr Quat operator+(const Quat& q) { return q; }

inline constexpr Quat operator-(const Quat& q) { return Quat(-q.w, -q.x, -q.y, -q.z); }

// Binary arithmetic operators
inline constexpr Quat operator+(const Quat& q, const Quat& p) { return Quat(q) += p; }

inline constexpr Quat operator-(const Quat& q, const Quat& p) { return Quat(q) -= p; }

inline constexpr Quat operator*(const Quat& q, const Quat& p) { return Quat(q) *= p; }

inline constexpr Vec3 operator*(const Quat& q, const Vec3& v) {
  const Vec3 axis(q.x, q.y, q.z);
  const Vec3 qv(cross(axis, v));
  const Vec3 qqv(cross(axis, qv));

  return v + 2.0F * ((q.w * qv) + qqv);
}

inline constexpr Vec3 operator*(const Vec3& v, const Quat& q) { return inverse(q) * v; }

inline constexpr Vec4 operator*(const Quat& q, const Vec4& v) { return Vec4(q * Vec3(v), v.w); }

inline constexpr Vec4 operator*(const Vec4& v, const Quat& q) { return inverse(q) * v; }

inline constexpr Quat operator*(const Quat& q, float scalar) {
  return Quat(q.w * scalar, q.x * scalar, q.y * scalar, q.z * scalar);
}

inline constexpr Quat operator*(float scalar, const Quat& q) { return q * scalar; }

inline constexpr Quat operator/(const Quat& q, float scalar) {
  return Quat(q.w / scalar, q.x / scalar, q.y / scalar, q.z / scalar);
}

// Boolean operators
inline constexpr bool operator==(const Quat& q1, const Quat& q2) {
  // TODO(C++23): Implement epsilon comparison
  return q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w;
}

inline constexpr bool operator!=(const Quat& q1, const Quat& q2) { return !(q1 == q2); }

}  // namespace mEn
