#pragma once

#include <mEn/mat3.hpp>
#include <mEn/quat.hpp>

namespace mEn {

[[nodiscard]] float length(const Quat& q);

[[nodiscard]] Quat normalize(const Quat& q);

[[nodiscard]] constexpr float dot(const Quat& u, const Quat& v);

[[nodiscard]] constexpr Quat cross(const Quat& u, const Quat& v);

[[nodiscard]] constexpr Mat3 mat3_cast(const Quat& q);

[[nodiscard]] constexpr Mat4 mat4_cast(const Quat& q);

[[nodiscard]] Quat quat_cast(const Mat3& m);

[[nodiscard]] Quat quat_cast(const Mat4& m);

[[nodiscard]] Quat quatLookAt(const Vec3& direction, const Vec3& up);

}  // namespace mEn

#include "quat_functions.inl"