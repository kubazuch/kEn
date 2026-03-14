
#include <mEn/functions/common.hpp>
#include <mEn/functions/exponential.hpp>

namespace mEn {

template <length_t L, typename T>
MEN_FORCE_INLINE T length(const vec<L, T>& x) noexcept {
  return sqrt(dot(x, x));
}

template <length_t L, typename T>
MEN_FORCE_INLINE T distance(const vec<L, T>& x, const vec<L, T>& y) noexcept {
  return length(x - y);
}

template <typename T>
MEN_FORCE_INLINE constexpr T dot(const vec<2, T>& x, const vec<2, T>& y) noexcept {
  vec<2, T> tmp(x * y);
  return tmp.x + tmp.y;
}

template <typename T>
MEN_FORCE_INLINE constexpr T dot(const vec<3, T>& x, const vec<3, T>& y) noexcept {
  vec<3, T> tmp(x * y);
  return tmp.x + tmp.y + tmp.z;
}

template <typename T>
MEN_FORCE_INLINE constexpr T dot(const vec<4, T>& x, const vec<4, T>& y) noexcept {
  vec<4, T> tmp(x * y);
  return tmp.x + tmp.y + tmp.z + tmp.w;
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T> cross(const vec<3, T>& x, const vec<3, T>& y) noexcept {
  return vec<3, T>{(x.y * y.z) - (y.y * x.z), (x.z * y.x) - (y.z * x.x), (x.x * y.y) - (y.x * x.y)};
}

template <length_t L, typename T>
MEN_FORCE_INLINE vec<L, T> normalize(const vec<L, T>& x) noexcept {
  return x * inversesqrt(dot(x, x));
}

template <length_t L, typename T>
MEN_FORCE_INLINE vec<L, T> faceforward(const vec<L, T>& N, const vec<L, T>& I, const vec<L, T>& Nref) noexcept {
  return dot(Nref, I) < T{0} ? N : -N;
}

template <length_t L, typename T>
MEN_FORCE_INLINE vec<L, T> reflect(const vec<L, T>& I, const vec<L, T>& N) noexcept {
  return I - (N * dot(N, I) * T{2});
}

template <length_t L, typename T>
MEN_FORCE_INLINE vec<L, T> refract(const vec<L, T>& I, const vec<L, T>& N, T eta) noexcept {
  const T dot_val = dot(N, I);
  const T k       = T{1} - (eta * eta * (T{1} - (dot_val * dot_val)));

  return k >= T{0} ? (eta * I) - (((eta * dot_val) + sqrt(k)) * N) : vec<L, T>(0);
}

}  // namespace mEn