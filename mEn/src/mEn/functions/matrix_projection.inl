#include <cmath>

#include <mEn/mat4.hpp>

namespace mEn {

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) noexcept {
  mat<4, T> ret(1);
  ret[0][0] = T{2} / (right - left);
  ret[1][1] = T{2} / (top - bottom);
  ret[2][2] = -T{2} / (zFar - zNear);
  ret[3][0] = -(right + left) / (right - left);
  ret[3][1] = -(top + bottom) / (top - bottom);
  ret[3][2] = -(zFar + zNear) / (zFar - zNear);

  return ret;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> perspective(T fov, T aspect, T zNear, T zFar) noexcept {
  const T tan_half_fov = std::tan(fov / T{2});

  mat<4, T> ret(0);
  ret[0][0] = T{1} / (tan_half_fov * aspect);
  ret[1][1] = T{1} / (tan_half_fov);
  ret[2][2] = -(zFar + zNear) / (zFar - zNear);
  ret[2][3] = -T{1};
  ret[3][2] = -(T{2} * zFar * zNear) / (zFar - zNear);

  return ret;
}

}  // namespace mEn