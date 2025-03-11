#pragma once

#include <cmath>

#include "vec_functions.hpp"
#include "quat_functions.hpp"

namespace mEn {

inline constexpr float dot(const Quat& u, const Quat& v) {
  vec<4, float> tmp(u.w * v.w, u.x * v.x, u.y * v.y, u.z * v.z);
  return tmp.x + tmp.y + tmp.z + tmp.w;
}

inline float length(const Quat& q) { return std::sqrt(dot(q, q)); }

inline Quat normalize(const Quat& q) {
  float len = length(q);
  if (len <= 0.0F) {
    return Quat(1.0F, 0.0F, 0.0F, 0.0F);
  }
  float inv_len = 1.0F / len;
  return Quat(q.w * inv_len, q.x * inv_len, q.y * inv_len, q.z * inv_len);
}

inline constexpr Quat cross(const Quat& u, const Quat& v) {
  return Quat(u.w * v.w - u.x * v.x - u.y * v.y - u.z * v.z,  //
              u.w * v.x + u.x * v.w + u.y * v.z - u.z * v.y,  //
              u.w * v.y + u.y * v.w + u.z * v.x - u.x * v.z,  //
              u.w * v.z + u.z * v.w + u.x * v.y - u.y * v.x);
}

inline constexpr Mat3 mat3_cast(const Quat& q) {
  Mat3 ret(1.F);

  ret[0][0] = 1.0F - 2.0F * (q.y * q.y + q.z * q.z);
  ret[0][1] = 2.0F * (q.x * q.y + q.w * q.z);
  ret[0][2] = 2.0F * (q.x * q.z - q.w * q.y);

  ret[1][0] = 2.0F * (q.x * q.y - q.w * q.z);
  ret[1][1] = 1.0F - 2.0F * (q.x * q.x + q.z * q.z);
  ret[1][2] = 2.0F * (q.y * q.z + q.w * q.x);

  ret[2][0] = 2.0F * (q.x * q.z + q.w * q.y);
  ret[2][1] = 2.0F * (q.y * q.z - q.w * q.x);
  ret[2][2] = 1.0F - 2.0F * (q.x * q.x + q.y * q.y);

  return ret;
}

inline constexpr Mat4 mat4_cast(const Quat& q) { return Mat4(mat3_cast(q)); }

inline Quat quat_cast(const Mat3& m) {
  // https://arxiv.org/pdf/math/0701759 formulas 20-29
  float k0 = m[0][0] + m[1][1] + m[2][2];
  float k1 = m[0][0] - m[1][1] - m[2][2];
  float k2 = m[1][1] - m[0][0] - m[2][2];
  float k3 = m[2][2] - m[0][0] - m[1][1];

  int max_index = 0;
  float k_max   = k0;
  if (k1 > k_max) {
    k_max     = k1;
    max_index = 1;
  }
  if (k2 > k_max) {
    k_max     = k2;
    max_index = 2;
  }
  if (k3 > k_max) {
    k_max     = k3;
    max_index = 3;
  }

  float max_val = sqrt(k_max + 1.0F) * 0.5F;
  float coeff   = 0.25F / max_val;

  switch (max_index) {
    case 0:
      return Quat(max_val, (m[1][2] - m[2][1]) * coeff, (m[2][0] - m[0][2]) * coeff, (m[0][1] - m[1][0]) * coeff);
    case 1:
      return Quat((m[1][2] - m[2][1]) * coeff, max_val, (m[0][1] + m[1][0]) * coeff, (m[2][0] + m[0][2]) * coeff);
    case 2:
      return Quat((m[2][0] - m[0][2]) * coeff, (m[0][1] + m[1][0]) * coeff, max_val, (m[1][2] + m[2][1]) * coeff);
    case 3:
    default:
      return Quat((m[0][1] - m[1][0]) * coeff, (m[2][0] + m[0][2]) * coeff, (m[1][2] + m[2][1]) * coeff, max_val);
  }
}

inline Quat quat_cast(const Mat4& m) { return quat_cast(Mat3(m)); }

inline Quat quatLookAt(const Vec3& direction, const Vec3& up) {
  Mat3 ret;

  ret[2]     = -direction;
  Vec3 right = cross(up, ret[2]);
  ret[0]     = right * (1.0F / sqrt(fmax(1e-5F, dot(right, right))));
  ret[1]     = cross(ret[2], ret[0]);

  return quat_cast(ret);
}

}  // namespace mEn