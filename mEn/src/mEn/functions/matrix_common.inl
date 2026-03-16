#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>
#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

namespace mEn {
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)

namespace detail {

template <length_t N, typename T, std::size_t... I>
constexpr mat<N, T> matrixCompMult(const mat<N, T>& x, const mat<N, T>& y, std::index_sequence<I...>) noexcept {
  return mat<N, T>{(x[I] * y[I])...};
}

template <length_t N, typename T, std::size_t... I>
constexpr mat<N, T> outerProduct(const vec<N, T>& c, const vec<N, T>& r, std::index_sequence<I...>) noexcept {
  return mat<N, T>{(c * r[I])...};
}

}  // namespace detail

template <length_t N, typename T>
MEN_FORCE_INLINE constexpr mat<N, T> matrixCompMult(const mat<N, T>& x, const mat<N, T>& y) noexcept {
  return detail::matrixCompMult(x, y, std::make_index_sequence<N>{});
}

template <length_t N, typename T>
MEN_FORCE_INLINE constexpr mat<N, T> outerProduct(const vec<N, T>& c, const vec<N, T>& r) noexcept {
  return detail::outerProduct(c, r, std::make_index_sequence<N>{});
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T> transpose(const mat<3, T>& m) noexcept {
  mat<3, T> res;

  res[0][0] = m[0][0];
  res[0][1] = m[1][0];
  res[0][2] = m[2][0];

  res[1][0] = m[0][1];
  res[1][1] = m[1][1];
  res[1][2] = m[2][1];

  res[2][0] = m[0][2];
  res[2][1] = m[1][2];
  res[2][2] = m[2][2];

  return res;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> transpose(const mat<4, T>& m) noexcept {
  mat<4, T> res;

  res[0][0] = m[0][0];
  res[0][1] = m[1][0];
  res[0][2] = m[2][0];
  res[0][3] = m[3][0];

  res[1][0] = m[0][1];
  res[1][1] = m[1][1];
  res[1][2] = m[2][1];
  res[1][3] = m[3][1];

  res[2][0] = m[0][2];
  res[2][1] = m[1][2];
  res[2][2] = m[2][2];
  res[2][3] = m[3][2];

  res[3][0] = m[0][3];
  res[3][1] = m[1][3];
  res[3][2] = m[2][3];
  res[3][3] = m[3][3];

  return res;
}

template <length_t N, typename T>
MEN_FORCE_INLINE constexpr T determinant(const mat<N, T>& m) noexcept {
  return detail::det(m);
}

template <length_t N, typename T>
MEN_FORCE_INLINE constexpr mat<N, T> inverse(const mat<N, T>& m) noexcept {
  return detail::inv(m);
}

// NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}  // namespace mEn