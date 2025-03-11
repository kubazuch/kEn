#pragma once

#ifdef MEN_GLM
#include <glm/gtx/quaternion.hpp>

namespace mEn {

using Quat = ::glm::quat;
using ::glm::mat4_cast;
using ::glm::normalize;
using ::glm::quatLookAt;

}  // namespace mEn
#else
#include "vec3.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

namespace mEn {

struct Quat {
  using value_t = float;

  float x, y, z, w;

  [[nodiscard]] static constexpr int length() { return 4; }

  // Components
  [[nodiscard]] constexpr float& operator[](int i);
  [[nodiscard]] constexpr const float& operator[](int i) const;

  // Implicit constructors
  constexpr Quat()              = default;
  constexpr Quat(const Quat& v) = default;

  // Explicit constructors
  constexpr Quat(float s, const Vec3& v);
  constexpr Quat(float w, float x, float y, float z);
  [[nodiscard]] static constexpr Quat wxyz(float w, float x, float y, float z);

  // Conversion constructors
  [[nodiscard]] explicit operator Mat3() const;
  [[nodiscard]] explicit operator Mat4() const;

  Quat(const Vec3& u, const Vec3& v);

  explicit Quat(const Vec3& eulerAngles);
  explicit Quat(const Mat3& m);
  explicit Quat(const Mat4& m);
};

}  // namespace mEn

#include "quat.inl"

#endif