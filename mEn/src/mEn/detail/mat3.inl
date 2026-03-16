namespace mEn {
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)

namespace detail {

template <typename T>
[[nodiscard]] MEN_FORCE_INLINE constexpr T det(const mat<3, T>& m) noexcept {
  const T a = m[0].x;
  const T d = m[0].y;
  const T g = m[0].z;

  const T b = m[1].x;
  const T e = m[1].y;
  const T h = m[1].z;

  const T c = m[2].x;
  const T f = m[2].y;
  const T i = m[2].z;

  // Laplace expansion
  return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

template <typename T>
[[nodiscard]] MEN_FORCE_INLINE constexpr mat<3, T> inv(const mat<3, T>& m) noexcept {
  const T a = m[0].x;
  const T d = m[0].y;
  const T g = m[0].z;

  const T b = m[1].x;
  const T e = m[1].y;
  const T h = m[1].z;

  const T c = m[2].x;
  const T f = m[2].y;
  const T i = m[2].z;

  const T det = detail::det(m);
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(det != static_cast<T>(0));
  }
  const T inv_det = T(1) / det;

  // Cofactors
  const T c00 = (e * i - f * h);
  const T c01 = -(d * i - f * g);
  const T c02 = (d * h - e * g);

  const T c10 = -(b * i - c * h);
  const T c11 = (a * i - c * g);
  const T c12 = -(a * h - b * g);

  const T c20 = (b * f - c * e);
  const T c21 = -(a * f - c * d);
  const T c22 = (a * e - b * d);

  return mat<3, T>{c00 * inv_det, c01 * inv_det, c02 * inv_det,  //
                   c10 * inv_det, c11 * inv_det, c12 * inv_det,  //
                   c20 * inv_det, c21 * inv_det, c22 * inv_det};
}

}  // namespace detail

// Components
template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T>::col_t& mat<3, T>::operator[](length_t i) noexcept {
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(i < length());
  }
  return value_[i];
}

template <typename T>
MEN_FORCE_INLINE constexpr const mat<3, T>::col_t& mat<3, T>::operator[](length_t i) const noexcept {
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(i < length());
  }
  return value_[i];
}

// Explicit constructors
template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T>::mat(T scalar) noexcept
    : value_{col_t{scalar, T{0}, T{0}}, col_t{T{0}, scalar, T{0}}, col_t{T{0}, {0}, scalar}} {}

template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T>::mat(T x0, T y0, T z0, T x1, T y1, T z1, T x2, T y2, T z2) noexcept
    : value_{col_t{x0, y0, z0}, col_t{x1, y1, z1}, col_t{x2, y2, z2}} {}

template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T>::mat(const col_t& v0, const col_t& v1, const col_t& v2) noexcept
    : value_{v0, v1, v2} {}

#if MEN_GLM
template <typename T>
template <Scalar U, glm::qualifier Q>
MEN_FORCE_INLINE constexpr mat<3, T>::mat(const glm::mat<3, 3, U, Q>& m) noexcept
    : value_{col_t{m[0]}, col_t{m[1]}, m[2]} {}

template <typename T>
template <glm::qualifier Q>
MEN_FORCE_INLINE constexpr mat<3, T>::operator glm::mat<3, 3, T, Q>() const noexcept {
  using gm_t  = glm::mat<3, 3, T, Q>;
  using col_g = gm_t::col_type;
  return gm_t{col_g{value_[0]}, col_g{value_[1]}, col_g{value_[2]}};
}
#endif

// Conversion constructors
template <typename T>
template <Scalar X0, Scalar Y0, Scalar Z0, Scalar X1, Scalar Y1, Scalar Z1, Scalar X2, Scalar Y2, Scalar Z2>
MEN_FORCE_INLINE constexpr mat<3, T>::mat(X0 x0, Y0 y0, Z0 z0, X1 x1, Y1 y1, Z1 z1, X2 x2, Y2 y2, Z2 z2) noexcept
    : value_{col_t{x0, y0, z0}, col_t{x1, y1, z1}, col_t{x2, y2, z2}} {}

template <typename T>
template <typename V1, typename V2, typename V3>
MEN_FORCE_INLINE constexpr mat<3, T>::mat(const vec<3, V1>& v1, const vec<3, V2>& v2, const vec<3, V3>& v3) noexcept
    : value_{col_t{v1}, col_t{v2}, col_t{v3}} {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<3, T>::mat(const mat<3, U>& m) noexcept
    : value_{col_t{m[0]}, col_t{m[1]}, col_t{m[2]}} {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<3, T>::mat(const mat<4, U>& m) noexcept
    : value_{col_t{m[0].x, m[0].y, m[0].z}, col_t{m[1].x, m[1].y, m[1].z}, col_t{m[2].x, m[2].y, m[2].z}} {}

// Unary arithmetic operators
template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator=(const mat<3, U>& m) noexcept {
  value_[0] = col_t{m[0]};
  value_[1] = col_t{m[1]};
  value_[2] = col_t{m[2]};
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator+=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  value_[0] += s;
  value_[1] += s;
  value_[2] += s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator+=(const mat<3, U>& m) noexcept {
  value_[0] += col_t{m[0]};
  value_[1] += col_t{m[1]};
  value_[2] += col_t{m[2]};
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator-=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  value_[0] -= s;
  value_[1] -= s;
  value_[2] -= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator-=(const mat<3, U>& m) noexcept {
  value_[0] -= col_t{m[0]};
  value_[1] -= col_t{m[1]};
  value_[2] -= col_t{m[2]};
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator*=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  value_[0] *= s;
  value_[1] *= s;
  value_[2] *= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator*=(const mat<3, U>& m) noexcept {
  *this = (*this) * mat<3, T>{m};
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator/=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  value_[0] /= s;
  value_[1] /= s;
  value_[2] /= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator/=(const mat<3, U>& m) noexcept {
  *this = (*this) / mat<3, T>{m};
  return *this;
}

// Increment and decrement operators
template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator++() noexcept {
  ++(*this)[0];
  ++(*this)[1];
  ++(*this)[2];
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T>& mat<3, T>::operator--() noexcept {
  --(*this)[0];
  --(*this)[1];
  --(*this)[2];
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T> mat<3, T>::operator++(int) noexcept {
  mat tmp{*this};
  ++(*this);
  return tmp;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T> mat<3, T>::operator--(int) noexcept {
  mat tmp{*this};
  --(*this);
  return tmp;
}

// Unary operators
template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T> operator+(mat<3, T> m) noexcept {
  return m;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T> operator-(const mat<3, T>& m) noexcept {
  return mat<3, T>{-m[0], -m[1], -m[2]};
}

// Scalar binary arithmetic operators
template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T> operator+(mat<3, T> m, U scalar) noexcept {
  m += scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T> operator+(U scalar, mat<3, T> m) noexcept {
  m += scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T> operator-(mat<3, T> m, U scalar) noexcept {
  m -= scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T> operator-(U scalar, const mat<3, T>& m) noexcept {
  mat<3, T> tmp{static_cast<T>(scalar)};
  tmp -= m;
  return tmp;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T> operator*(mat<3, T> m, U scalar) noexcept {
  m *= scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T> operator*(U scalar, mat<3, T> m) noexcept {
  m *= scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T> operator/(mat<3, T> m, U scalar) noexcept {
  m /= scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<3, T> operator/(U scalar, const mat<3, T>& m) noexcept {
  return static_cast<T>(scalar) * detail::inv(m);
}

// Matrix binary arithmetic operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<3, T> operator+(mat<3, T> lhs, const mat<3, U>& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<3, T> operator-(mat<3, T> lhs, const mat<3, U>& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<3, T> operator*(const mat<3, T>& lhs, const mat<3, U>& rhs) noexcept {
  return mat<3, T>{lhs * rhs[0], lhs * rhs[1], lhs * rhs[2]};
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<3, T> operator/(mat<3, T> lhs, const mat<3, U>& rhs) noexcept {
  lhs *= detail::inv(mat<3, T>{rhs});
  return lhs;
}

// Matrix / vector operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<3, T>::col_t operator*(const mat<3, T>& m, const vec<3, U>& v) noexcept {
  const T x = static_cast<T>(v[0]);
  const T y = static_cast<T>(v[1]);
  const T z = static_cast<T>(v[2]);
  return (m[0] * x) + (m[1] * y) + (m[2] * z);
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<3, T>::row_t operator*(const vec<3, U>& v, const mat<3, T>& m) noexcept {
  const T x = static_cast<T>(v[0]);
  const T y = static_cast<T>(v[1]);
  const T z = static_cast<T>(v[2]);

  return typename mat<3, T>::row_t{
      x * m[0][0] + y * m[0][1] + z * m[0][2],
      x * m[1][0] + y * m[1][1] + z * m[1][2],
      x * m[2][0] + y * m[2][1] + z * m[2][2],
  };
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<3, T>::col_t operator/(const mat<3, T>& m, const vec<3, U>& v) noexcept {
  return detail::inv(m) * vec<3, T>{v};
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<3, T>::row_t operator/(const vec<3, U>& v, const mat<3, T>& m) noexcept {
  return vec<3, T>{v} * detail::inv(m);
}

// Boolean operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator==(const mat<3, T>& m1, const mat<3, U>& m2) noexcept {
  return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]);
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator!=(const mat<3, T>& m1, const mat<3, U>& m2) noexcept {
  return !(m1 == m2);
}

// NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}  // namespace mEn
