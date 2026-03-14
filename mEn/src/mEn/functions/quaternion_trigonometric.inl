#include <mEn/constants.hpp>
#include <mEn/functions/common.hpp>
#include <mEn/functions/exponential.hpp>
#include <mEn/functions/trigonometric.hpp>

namespace mEn {  // NOLINT

template <typename T>
MEN_FORCE_INLINE T angle(const qua<T>& q) noexcept {
  if (abs(q.w) > T{0.877582561890372716130286068203503191}) {
    const T angle = T{2} * asin(sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z)));

    return q.w < T{0} ? kTwoPi<T> - angle : angle;
  }

  return T{2} * acos(q.w);
}

template <typename T>
MEN_FORCE_INLINE vec<3, T> axis(const qua<T>& q) noexcept {
  const T sin_half_angle_sq = T{1} - (q.w * q.w);

  if (sin_half_angle_sq <= T{0}) {
    return vec<3, T>(0, 0, 1);
  }

  const T inv_sin_half_angle = T{1} / sqrt(sin_half_angle_sq);
  return vec<3, T>(q.x * inv_sin_half_angle, q.y * inv_sin_half_angle, q.z * inv_sin_half_angle);
}

template <typename T>
MEN_FORCE_INLINE qua<T> angleAxis(T angle, const vec<3, T>& axis) noexcept {
  const T sine = sin(T{0.5} * angle);

  return qua<T>(cos(T{0.5} * angle), axis * sine);
}

template <typename T>
MEN_FORCE_INLINE vec<3, T> eulerAngles(const qua<T>& q) noexcept {
  return vec<3, T>(pitch(q), yaw(q), roll(q));
}

template <typename T>
MEN_FORCE_INLINE T roll(const qua<T>& q) noexcept {
  const T y = T{2} * ((q.x * q.y) + (q.w * q.z));
  const T x = (q.w * q.w) + (q.x * q.x) - (q.y * q.y) - (q.z * q.z);

  if (abs(x) < T{1e-6} && abs(y) < T{1e-6}) {
    return T{0};
  }

  return atan2(y, x);
}

template <typename T>
MEN_FORCE_INLINE T pitch(const qua<T>& q) noexcept {
  const T y = T{2} * ((q.y * q.z) + (q.w * q.x));
  const T x = (q.w * q.w) - (q.x * q.x) - (q.y * q.y) + (q.z * q.z);

  if (abs(x) < T{1e-6} && abs(y) < T{1e-6}) {
    return T{2} * atan(q.x, q.w);
  }

  return atan2(y, x);
}

template <typename T>
MEN_FORCE_INLINE T yaw(const qua<T>& q) noexcept {
  return asin(clamp(T{-2} * ((q.x * q.z) - (q.w * q.y)), T{-1}, T{1}));
}

}  // namespace mEn