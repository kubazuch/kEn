#pragma once

#include <mEn/mat3.hpp>
#include <mEn/quat.hpp>

namespace mEn {

// Geometric
[[nodiscard]] float length(const Quat& q);

[[nodiscard]] Quat normalize(const Quat& q);

[[nodiscard]] constexpr float dot(const Quat& u, const Quat& v);

[[nodiscard]] constexpr Quat cross(const Quat& u, const Quat& v);

[[nodiscard]] Quat quatLookAt(const Vec3& direction, const Vec3& up);

[[nodiscard]] Quat mix(const Quat& q, const Quat& p, float t);

[[nodiscard]] constexpr Quat lerp(const Quat& q, const Quat& p, float t);

[[nodiscard]] Quat slerp(const Quat& q, const Quat& p, float t);

[[nodiscard]] constexpr Quat conjugate(const Quat& q);

[[nodiscard]] constexpr Quat inverse(const Quat& q);

// Cast
[[nodiscard]] constexpr Mat3 mat3_cast(const Quat& q);

[[nodiscard]] constexpr Mat4 mat4_cast(const Quat& q);

[[nodiscard]] Quat quat_cast(const Mat3& m);

[[nodiscard]] Quat quat_cast(const Mat4& m);

}  // namespace mEn

#include "quat_functions.inl"