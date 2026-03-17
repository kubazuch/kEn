#include <mEn/constants.hpp>
#include <mEn/functions/geometric.hpp>
#include <mEn/functions/matrix_common.hpp>
#include <mEn/functions/quaternion_geometric.hpp>
#include <mEn/mat4.hpp>
#include <mEn/quat.hpp>
#include <mEn/vec3.hpp>

namespace mEn {

template <typename T>
MEN_FORCE_INLINE bool decompose(const mat<4, T>& model, vec<3, T>& translation, qua<T>& rotation,
                                vec<3, T>& scale) noexcept {
  const T eps = kEpsilon<T>;

  if (abs(model[3][3]) < eps) {
    return false;
  }

  if (abs(model[0][3]) > eps || abs(model[1][3]) > eps || abs(model[2][3]) > eps) {
    return false;
  }

  translation = vec<3, T>(model[3]);

  vec<3, T> x(model[0]);
  vec<3, T> y(model[1]);
  vec<3, T> z(model[2]);

  scale.x = length(x);
  scale.y = length(y);
  scale.z = length(z);

  if (abs(scale.x) < eps || abs(scale.y) < eps || abs(scale.z) < eps) {
    return false;
  }

  x /= scale.x;
  y /= scale.y;
  z /= scale.z;

  mat<3, T> r(x, y, z);

  if (determinant(r) < T{0}) {
    scale.x = -scale.x;
    r[0]    = -r[0];
  }

  rotation = normalize(qua<T>(r));
  return true;
}

}  // namespace mEn