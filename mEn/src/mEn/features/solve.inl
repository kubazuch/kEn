#include <mEn/constants.hpp>
#include <mEn/functions/common.hpp>
#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

namespace mEn {

template <length_t N, typename T>
MEN_FORCE_INLINE bool solve(const mat<N, T>& a, const vec<N, T>& b, vec<N, T>& x) noexcept {
  const T eps = kEpsilon<T>;

  mat<N, T> m   = a;  // column-major: m[col][row]
  vec<N, T> rhs = b;

  // Forward elimination with partial pivoting.
  for (length_t col = 0; col < N; ++col) {
    // Pick the row with the largest magnitude in this column, at or below the diagonal.
    length_t pivot = col;
    T best         = abs(m[col][col]);
    for (length_t row = col + 1; row < N; ++row) {
      const T mag = abs(m[col][row]);
      if (mag > best) {
        best  = mag;
        pivot = row;
      }
    }

    if (best < eps) {
      return false;  // singular (no usable pivot)
    }

    // Bring the pivot row into place. Columns left of `col` are already eliminated,
    // so only columns >= col and the right-hand side need swapping.
    if (pivot != col) {
      for (length_t c = col; c < N; ++c) {
        const T tmp = m[c][col];
        m[c][col]   = m[c][pivot];
        m[c][pivot] = tmp;
      }
      const T tmp = rhs[col];
      rhs[col]    = rhs[pivot];
      rhs[pivot]  = tmp;
    }

    // Eliminate the entries below the pivot.
    const T diag = m[col][col];
    for (length_t row = col + 1; row < N; ++row) {
      const T factor = m[col][row] / diag;
      for (length_t c = col; c < N; ++c) {
        m[c][row] -= factor * m[c][col];
      }
      rhs[row] -= factor * rhs[col];
    }
  }

  // Back substitution on the upper-triangular system.
  for (length_t i = N; i-- > 0;) {
    T sum = rhs[i];
    for (length_t c = i + 1; c < N; ++c) {
      sum -= m[c][i] * x[c];
    }
    x[i] = sum / m[i][i];
  }

  return true;
}

}  // namespace mEn
