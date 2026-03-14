#include <cmath>

#include <mEn/constants.hpp>
#include <mEn/functions/quaternion_geometric.hpp>
#include <mEn/quat.hpp>

namespace mEn {

template <typename T>
MEN_FORCE_INLINE qua<T> mix(const qua<T>& x, const qua<T>& y, T t) noexcept {
  const T cos = dot(x, y);

  if (cos > T{1} - kEpsilon<T>) {
    return qua<T>(((T{1} - t) * x.w) + (t * y.w),  //
                  ((T{1} - t) * x.x) + (t * y.x),  //
                  ((T{1} - t) * x.y) + (t * y.y),  //
                  ((T{1} - t) * x.z) + (t * y.z));
  }

  T angle = std::acos(cos);
  return ((sin((T{1} - t) * angle) * x) + (sin(t * angle) * y)) / sin(angle);
}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> lerp(const qua<T>& x, const qua<T>& y, T t) noexcept {
  return ((T{1} - t) * x) + (t * y);
}

template <typename T>
MEN_FORCE_INLINE qua<T> slerp(const qua<T>& x, const qua<T>& y, T t) noexcept {
  qua<T> z = y;
  T cos    = dot(x, y);

  if (cos < T{0}) {
    z   = -y;
    cos = -cos;
  }

  if (cos > T{1} - kEpsilon<T>) {
    return qua<T>(((T{1} - t) * x.w) + (t * z.w),  //
                  ((T{1} - t) * x.x) + (t * z.x),  //
                  ((T{1} - t) * x.y) + (t * z.y),  //
                  ((T{1} - t) * x.z) + (t * z.z));
  }

  T angle = std::acos(cos);
  return ((sin((T{1} - t) * angle) * x) + (sin(t * angle) * z)) / sin(angle);
}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> conjugate(const qua<T>& q) noexcept {
  return qua<T>(q.w, -q.x, -q.y, -q.z);
}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> inverse(const qua<T>& q) noexcept {
  return detail::inverse(q);
}

template <typename T>
MEN_FORCE_INLINE vec<4, bool> isnan(const qua<T>& q) noexcept {
  return vec<4, bool>(std::isnan(q.x), std::isnan(q.y), std::isnan(q.z), std::isnan(q.w));
}

template <typename T>
MEN_FORCE_INLINE vec<4, bool> isinf(const qua<T>& q) noexcept {
  return vec<4, bool>(std::isinf(q.x), std::isinf(q.y), std::isinf(q.z), std::isinf(q.w));
}

}  // namespace mEn