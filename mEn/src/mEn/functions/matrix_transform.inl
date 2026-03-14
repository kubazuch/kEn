#include <mEn/functions/geometric.hpp>
#include <mEn/functions/trigonometric.hpp>
#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>
#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>

namespace mEn {

template <length_t N, typename T>
MEN_FORCE_INLINE constexpr mat<N, T> identity() noexcept {
  return mat<N, T>(1);
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> translate(const mat<4, T>& m, const vec<3, T>& v) noexcept {
  mat<4, T> ret(m);
  ret[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
  return ret;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> rotate(const mat<4, T>& m, T angle, const vec<3, T>& v) noexcept {
  const T a = angle;
  const T c = mEn::cos(a);
  const T s = mEn::sin(a);

  vec<3, T> axis(normalize(v));
  vec<3, T> tmp((T{1} - c) * axis);

  mat<4, T> rot;
  rot[0][0] = c + tmp[0] * axis[0];
  rot[0][1] = tmp[0] * axis[1] + s * axis[2];
  rot[0][2] = tmp[0] * axis[2] - s * axis[1];

  rot[1][0] = tmp[1] * axis[0] - s * axis[2];
  rot[1][1] = c + tmp[1] * axis[1];
  rot[1][2] = tmp[1] * axis[2] + s * axis[0];

  rot[2][0] = tmp[2] * axis[0] + s * axis[1];
  rot[2][1] = tmp[2] * axis[1] - s * axis[0];
  rot[2][2] = c + tmp[2] * axis[2];

  mat<4, T> ret;
  ret[0] = m[0] * rot[0][0] + m[1] * rot[0][1] + m[2] * rot[0][2];
  ret[1] = m[0] * rot[1][0] + m[1] * rot[1][1] + m[2] * rot[1][2];
  ret[2] = m[0] * rot[2][0] + m[1] * rot[2][1] + m[2] * rot[2][2];
  ret[3] = m[3];
  return ret;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> scale(const mat<4, T>& m, const vec<3, T>& v) noexcept {
  mat<4, T> ret;
  ret[0] = m[0] * v[0];
  ret[1] = m[1] * v[1];
  ret[2] = m[2] * v[2];
  ret[3] = m[3];
  return ret;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> shear(const mat<4, T>& m, const vec<3, T>& p, const vec<2, T>& l_x,
                                           const vec<2, T>& l_y, const vec<2, T>& l_z) noexcept {
  const T lambda_xy = l_x[0];
  const T lambda_xz = l_x[1];
  const T lambda_yx = l_y[0];
  const T lambda_yz = l_y[1];
  const T lambda_zx = l_z[0];
  const T lambda_zy = l_z[1];

  vec<3, T> point_lambda{lambda_xy + lambda_xz, lambda_yx + lambda_yz, lambda_zx + lambda_zy};

  mat<4, T> shr{1,
                lambda_yx,
                lambda_zx,
                0,
                lambda_xy,
                1,
                lambda_zy,
                0,
                lambda_xz,
                lambda_yz,
                1,
                0,
                -point_lambda[0] * p[0],
                -point_lambda[1] * p[1],
                -point_lambda[2] * p[2],
                1};

  mat<4, T> ret;
  ret[0] = m[0] * shr[0][0] + m[1] * shr[0][1] + m[2] * shr[0][2] + m[3] * shr[0][3];
  ret[1] = m[0] * shr[1][0] + m[1] * shr[1][1] + m[2] * shr[1][2] + m[3] * shr[1][3];
  ret[2] = m[0] * shr[2][0] + m[1] * shr[2][1] + m[2] * shr[2][2] + m[3] * shr[2][3];
  ret[3] = m[0] * shr[3][0] + m[1] * shr[3][1] + m[2] * shr[3][2] + m[3] * shr[3][3];
  return ret;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> lookAt(const vec<3, T>& eye, const vec<3, T>& center,
                                            const vec<3, T>& up) noexcept {
  const vec<3, T> f(normalize(center - eye));
  const vec<3, T> s(normalize(cross(f, up)));
  const vec<3, T> u(cross(s, f));

  mat<4, T> ret(1);
  ret[0][0] = s.x;
  ret[1][0] = s.y;
  ret[2][0] = s.z;
  ret[0][1] = u.x;
  ret[1][1] = u.y;
  ret[2][1] = u.z;
  ret[0][2] = -f.x;
  ret[1][2] = -f.y;
  ret[2][2] = -f.z;
  ret[3][0] = -dot(s, eye);
  ret[3][1] = -dot(u, eye);
  ret[3][2] = dot(f, eye);
  return ret;
}

}  // namespace mEn