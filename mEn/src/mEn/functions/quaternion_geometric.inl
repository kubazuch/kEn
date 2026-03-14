#include <cmath>

#include <mEn/quat.hpp>

namespace mEn {

template <typename T>
MEN_FORCE_INLINE constexpr T dot(const qua<T>& x, const qua<T>& y) noexcept {
  return x.w * y.w + x.x * y.x + x.y * y.y + x.z * y.z;
}

template <typename T>
MEN_FORCE_INLINE T length(const qua<T>& q) noexcept {
  return std::sqrt(dot(q, q));
}

template <typename T>
MEN_FORCE_INLINE qua<T> normalize(const qua<T>& q) noexcept {
  T len = length(q);
  if (len <= 0.0F) {
    return qua<T>(T{1}, T{0}, T{0}, T{0});
  }

  T inv_len = T{1} / len;
  return qua<T>(q.w * inv_len, q.x * inv_len, q.y * inv_len, q.z * inv_len);
}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> cross(const qua<T>& x, const qua<T>& y) noexcept {
  return x * y;
}

}  // namespace mEn