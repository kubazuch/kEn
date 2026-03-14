#include <mEn/functions/geometric.hpp>
#include <mEn/functions/trigonometric.hpp>
#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>
#include <mEn/vec3.hpp>

namespace mEn {

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> identity() noexcept {
  return qua<T>(T{1}, T{0}, T{0}, T{0});
}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> rotate(const qua<T>& q, T angle, const vec<3, T>& v) noexcept {
  const T a = angle;
  const T c = mEn::cos(T{0.5} * a);
  const T s = mEn::sin(T{0.5} * a);

  vec<3, T> axis(normalize(v));
  return q * qua<T>(c, axis.x * s, axis.y * s, axis.z * s);
}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> quatLookAt(const vec<3, T>& eye, const vec<3, T>& center,
                                             const vec<3, T>& up) noexcept {
  const vec<3, T> f(normalize(center - eye));
  const vec<3, T> s(normalize(cross(up, f)));
  const vec<3, T> u(cross(f, s));

  mat<3, T> ret;
  ret[0] = s;
  ret[1] = u;
  ret[2] = f;

  return qua<T>(ret);
}

}  // namespace mEn