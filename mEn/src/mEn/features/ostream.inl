
#include <ostream>

#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>
#include <mEn/quat.hpp>
#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

namespace mEn {
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)

template <length_t L, typename T>
MEN_FORCE_INLINE std::ostream& operator<<(std::ostream& os, const vec<L, T>& v) {
  os << '(';
  for (length_t i = 0; i < L; ++i) {
    if (i > 0) {
      os << ", ";
    }
    os << v[i];
  }
  return os << ')';
}

template <length_t N, typename T>
MEN_FORCE_INLINE std::ostream& operator<<(std::ostream& os, const mat<N, T>& m) {
  os << '(';
  for (length_t r = 0; r < N; ++r) {
    if (r > 0) {
      os << ", ";
    }
    os << '(';
    for (length_t c = 0; c < N; ++c) {
      if (c > 0) {
        os << ", ";
      }
      os << m[c][r];
    }
    os << ')';
  }
  return os << ')';
}

template <typename T>
MEN_FORCE_INLINE std::ostream& operator<<(std::ostream& os, const qua<T>& q) {
  return os << '(' << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ')';
}

// NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}  // namespace mEn
