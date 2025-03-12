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
#include "mat3.hpp"
#include "mat4.hpp"
#include "vec3.hpp"

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

  // Unary arithmetic operators
  constexpr Quat& operator=(const Quat& q) = default;

  constexpr Quat& operator+=(const Quat& q);
  constexpr Quat& operator-=(const Quat& q);
  template <typename U>
  constexpr Quat& operator*=(U scalar);
  constexpr Quat& operator*=(const Quat& q);
  template <typename U>
  constexpr Quat& operator/=(U scalar);
};

// Unary operators
[[nodiscard]] constexpr Quat operator+(const Quat& q);
[[nodiscard]] constexpr Quat operator-(const Quat& q);

// Binary arithmetic operators
[[nodiscard]] constexpr Quat operator+(const Quat& q, const Quat& p);
[[nodiscard]] constexpr Quat operator-(const Quat& q, const Quat& p);
[[nodiscard]] constexpr Quat operator*(const Quat& q, const Quat& p);
[[nodiscard]] constexpr Vec3 operator*(const Quat& q, const Vec3& v);
[[nodiscard]] constexpr Vec3 operator*(const Vec3& v, const Quat& q);
[[nodiscard]] constexpr Vec4 operator*(const Quat& q, const Vec4& v);
[[nodiscard]] constexpr Vec4 operator*(const Vec4& v, const Quat& q);
[[nodiscard]] constexpr Quat operator*(const Quat& q, float scalar);
[[nodiscard]] constexpr Quat operator*(float scalar, const Quat& q);
[[nodiscard]] constexpr Quat operator/(const Quat& q, float scalar);

// Boolean operators
[[nodiscard]] constexpr bool operator==(const Quat& q1, const Quat& q2);
template <typename T>
[[nodiscard]] constexpr bool operator!=(const Quat& q1, const Quat& q2);

}  // namespace mEn

#include "quat.inl"

#endif