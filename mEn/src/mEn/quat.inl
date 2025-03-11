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

}  // namespace mEn
