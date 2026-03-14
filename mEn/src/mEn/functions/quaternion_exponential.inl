
#include <mEn/constants.hpp>
#include <mEn/functions/exponential.hpp>
#include <mEn/functions/trigonometric.hpp>
#include <mEn/quat.hpp>

namespace mEn {  // NOLINT

template <typename T>
MEN_FORCE_INLINE qua<T> exp(const qua<T>& q) noexcept {
  vec<3, T> u(q.x, q.y, q.z);
  const T angle = length(u);
  const T scale = exp(q.w);
  if (angle < kEpsilon<T>) {
    return scale * qua<T>(1, 0, 0, 0);
  }

  const vec<3, T> v(u / angle);
  return qua<T>(scale * cos(angle), scale * sin(angle) * v);
}

template <typename T>
MEN_FORCE_INLINE qua<T> log(const qua<T>& q) noexcept {
  vec<3, T> u(q.x, q.y, q.z);
  const T len = length(u);

  if (len < kEpsilon<T>) {
    if (q.w > T{0}) {
      return qua<T>(log(q.w), T{0}, T{0}, T{0});
    }
    if (q.w < T{0}) {
      return qua<T>(log(-q.w), kPi<T>, T{0}, T{0});
    }
    return qua<T>(kNaN<T>, kNaN<T>, kNaN<T>, kNaN<T>);
  }

  const T t            = atan(len, T{q.w}) / len;
  const T quat_norm_sq = len * len + q.w * q.w;
  return qua<T>(T{0.5} * log(quat_norm_sq), t * q.x, t * q.y, t * q.z);
}

template <typename T>
MEN_FORCE_INLINE qua<T> pow(const qua<T>& q, T x) noexcept {
  if (abs(x) < kEpsilon<T>) {
    return qua<T>(T{1}, T{0}, T{0}, T{0});
  }

  const T vec_norm_sq = q.x * q.x + q.y * q.y + q.z * q.z;
  const T norm_sq     = vec_norm_sq + q.w * q.w;

  if (norm_sq < std::numeric_limits<T>::min()) {
    return qua<T>(T{0}, T{0}, T{0}, T{0});
  }

  const T norm = sqrt(norm_sq);

  T angle;
  if (abs(q.w / norm) > T{0.877582561890372716130286068203503191}) {
    if (vec_norm_sq < std::numeric_limits<T>::min()) {
      return qua<T>(pow(q.w, x), 0, 0, 0);
    }

    angle = asin(sqrt(vec_norm_sq) / norm);
    if (q.w < T{0}) {
      angle = kTwoPi<T> - angle;
    }
  } else {
    angle = acos(q.w / norm);
  }

  T new_angle = angle * x;
  T factor    = sin(new_angle) / sin(angle);
  T scale     = pow(norm, x - T{1});
  return qua<T>(cos(new_angle) * norm * scale, q.x * factor * scale, q.y * factor * scale, q.z * factor * scale);
}

template <typename T>
MEN_FORCE_INLINE qua<T> sqrt(const qua<T>& q) noexcept {
  return pow(q, T{0.5});
}

}  // namespace mEn