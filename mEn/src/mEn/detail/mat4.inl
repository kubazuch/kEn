namespace mEn {

namespace detail {

template <typename T>
[[nodiscard]] MEN_FORCE_INLINE constexpr T det(const mat<4, T>& m) noexcept {
  // Laplace expansion applied twice
  const T minor23 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
  const T minor13 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
  const T minor12 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
  const T minor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
  const T minor02 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
  const T minor01 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

  const T cof00 = +(m[1][1] * minor23 - m[1][2] * minor13 + m[1][3] * minor12);
  const T cof10 = -(m[1][0] * minor23 - m[1][2] * minor03 + m[1][3] * minor02);
  const T cof20 = +(m[1][0] * minor13 - m[1][1] * minor03 + m[1][3] * minor01);
  const T cof30 = -(m[1][0] * minor12 - m[1][1] * minor02 + m[1][2] * minor01);

  return m[0][0] * cof00 + m[0][1] * cof10 + m[0][2] * cof20 + m[0][3] * cof30;
}

template <typename T>
[[nodiscard]] MEN_FORCE_INLINE constexpr mat<4, T> inv(const mat<4, T>& m) noexcept {
  const T minor123 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
  const T minor223 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
  const T minor323 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

  const T minor113 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
  const T minor213 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
  const T minor313 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

  const T minor112 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
  const T minor212 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
  const T minor312 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

  const T minor103 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
  const T minor203 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
  const T minor303 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

  const T minor102 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
  const T minor202 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
  const T minor302 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

  const T minor101 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
  const T minor201 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
  const T minor301 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

  const vec<4, T> factors23(minor123, minor123, minor223, minor323);
  const vec<4, T> factors13(minor113, minor113, minor213, minor313);
  const vec<4, T> factors12(minor112, minor112, minor212, minor312);
  const vec<4, T> factors03(minor103, minor103, minor203, minor303);
  const vec<4, T> factors02(minor102, minor102, minor202, minor302);
  const vec<4, T> factors01(minor101, minor101, minor201, minor301);

  const vec<4, T> multipliers0(m[1][0], m[0][0], m[0][0], m[0][0]);
  const vec<4, T> multipliers1(m[1][1], m[0][1], m[0][1], m[0][1]);
  const vec<4, T> multipliers2(m[1][2], m[0][2], m[0][2], m[0][2]);
  const vec<4, T> multipliers3(m[1][3], m[0][3], m[0][3], m[0][3]);

  const vec<4, T> cofactors0(multipliers1 * factors23 - multipliers2 * factors13 + multipliers3 * factors12);
  const vec<4, T> cofactors1(multipliers0 * factors23 - multipliers2 * factors03 + multipliers3 * factors02);
  const vec<4, T> cofactors2(multipliers0 * factors13 - multipliers1 * factors03 + multipliers3 * factors01);
  const vec<4, T> cofactors3(multipliers0 * factors12 - multipliers1 * factors02 + multipliers2 * factors01);

  const vec<4, T> signs1(+1, -1, +1, -1);
  const vec<4, T> signs2(-1, +1, -1, +1);
  mat<4, T> inverse(cofactors0 * signs1, cofactors1 * signs2, cofactors2 * signs1, cofactors3 * signs2);

  const vec<4, T> row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

  const vec<4, T> dot0(m[0] * row0);
  T dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

  T inv_det = static_cast<T>(1) / dot1;

  return inverse * inv_det;
}

}  // namespace detail

// Components
template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T>::col_t& mat<4, T>::operator[](length_t i) noexcept {
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(i < length());
  }
  return value_[i];
}

template <typename T>
MEN_FORCE_INLINE constexpr const mat<4, T>::col_t& mat<4, T>::operator[](length_t i) const noexcept {
  if (!std::is_constant_evaluated()) {
    MEN_ASSERT(i < length());
  }
  return value_[i];
}

// Explicit constructors
template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T>::mat(T scalar) noexcept
    : value_{col_t{scalar, T{0}, T{0}, T{0}}, col_t{T{0}, scalar, T{0}, T{0}}, col_t{T{0}, T{0}, scalar, T{0}},
             col_t{T{0}, T{0}, T{0}, scalar}} {}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T>::mat(T x0, T y0, T z0, T w0,  //
                                          T x1, T y1, T z1, T w1,  //
                                          T x2, T y2, T z2, T w2,  //
                                          T x3, T y3, T z3, T w3) noexcept
    : value_{col_t{x0, y0, z0, w0}, col_t{x1, y1, z1, w1}, col_t{x2, y2, z2, w2}, col_t{x3, y3, z3, w3}} {}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T>::mat(const col_t& v0, const col_t& v1, const col_t& v2, const col_t& v3) noexcept
    : value_{v0, v1, v2, v3} {}

#if MEN_GLM
template <typename T>
template <Scalar U, glm::qualifier Q>
MEN_FORCE_INLINE constexpr mat<4, T>::mat(const glm::mat<4, 4, U, Q>& m) noexcept {
  value_[0] = col_t{m[0]};
  value_[1] = col_t{m[1]};
  value_[2] = col_t{m[2]};
  value_[3] = col_t{m[3]};
}

template <typename T>
template <glm::qualifier Q>
MEN_FORCE_INLINE constexpr mat<4, T>::operator glm::mat<4, 4, T, Q>() const noexcept {
  using gm_t  = glm::mat<4, 4, T, Q>;
  using col_g = gm_t::col_type;
  return gm_t{col_g{value_[0]}, col_g{value_[1]}, col_g{value_[2]}, col_g{value_[3]}};
}
#endif

// Conversion constructors
template <typename T>
template <Scalar X0, Scalar Y0, Scalar Z0, Scalar W0,  //
          Scalar X1, Scalar Y1, Scalar Z1, Scalar W1,  //
          Scalar X2, Scalar Y2, Scalar Z2, Scalar W2,  //
          Scalar X3, Scalar Y3, Scalar Z3, Scalar W3>
MEN_FORCE_INLINE constexpr mat<4, T>::mat(X0 x0, Y0 y0, Z0 z0, W0 w0,  //
                                          X1 x1, Y1 y1, Z1 z1, W1 w1,  //
                                          X2 x2, Y2 y2, Z2 z2, W2 w2,  //
                                          X3 x3, Y3 y3, Z3 z3, W3 w3) noexcept
    : value_{col_t{x0, y0, z0, w0}, col_t{x1, y1, z1, w1}, col_t{x2, y2, z2, w2}, col_t{x3, y3, z3, w3}} {}

template <typename T>
template <typename V1, typename V2, typename V3, typename V4>
MEN_FORCE_INLINE constexpr mat<4, T>::mat(const vec<4, V1>& v1, const vec<4, V2>& v2, const vec<4, V3>& v3,
                                          const vec<4, V4>& v4) noexcept
    : value_{col_t{v1}, col_t{v2}, col_t{v3}, col_t{v4}} {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<4, T>::mat(const mat<3, U>& m) noexcept
    : value_{col_t{static_cast<T>(m[0][0]), static_cast<T>(m[0][1]), static_cast<T>(m[0][2]), T{0}},
             col_t{static_cast<T>(m[1][0]), static_cast<T>(m[1][1]), static_cast<T>(m[1][2]), T{0}},
             col_t{static_cast<T>(m[2][0]), static_cast<T>(m[2][1]), static_cast<T>(m[2][2]), T{0}},
             col_t{T{0}, T{0}, T{0}, T{1}}} {}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<4, T>::mat(const mat<4, U>& m) noexcept
    : value_{col_t{m[0]}, col_t{m[1]}, col_t{m[2]}, col_t{m[3]}} {}

// Unary arithmetic operators
template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator=(const mat<4, U>& m) noexcept {
  value_[0] = col_t{m[0]};
  value_[1] = col_t{m[1]};
  value_[2] = col_t{m[2]};
  value_[3] = col_t{m[3]};
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator+=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  value_[0] += s;
  value_[1] += s;
  value_[2] += s;
  value_[3] += s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator+=(const mat<4, U>& m) noexcept {
  value_[0] += col_t{m[0]};
  value_[1] += col_t{m[1]};
  value_[2] += col_t{m[2]};
  value_[3] += col_t{m[3]};
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator-=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  value_[0] -= s;
  value_[1] -= s;
  value_[2] -= s;
  value_[3] -= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator-=(const mat<4, U>& m) noexcept {
  value_[0] -= col_t{m[0]};
  value_[1] -= col_t{m[1]};
  value_[2] -= col_t{m[2]};
  value_[3] -= col_t{m[3]};
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator*=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  value_[0] *= s;
  value_[1] *= s;
  value_[2] *= s;
  value_[3] *= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator*=(const mat<4, U>& m) noexcept {
  *this = (*this) * mat<4, T>{m};
  return *this;
}

template <typename T>
template <Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator/=(U scalar) noexcept {
  const T s = static_cast<T>(scalar);
  value_[0] /= s;
  value_[1] /= s;
  value_[2] /= s;
  value_[3] /= s;
  return *this;
}

template <typename T>
template <typename U>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator/=(const mat<4, U>& m) noexcept {
  *this = (*this) / mat<4, T>{m};
  return *this;
}

// Increment and decrement operators
template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator++() noexcept {
  ++(*this)[0];
  ++(*this)[1];
  ++(*this)[2];
  ++(*this)[3];
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T>& mat<4, T>::operator--() noexcept {
  --(*this)[0];
  --(*this)[1];
  --(*this)[2];
  --(*this)[3];
  return *this;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> mat<4, T>::operator++(int) noexcept {
  mat tmp{*this};
  ++(*this);
  return tmp;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> mat<4, T>::operator--(int) noexcept {
  mat tmp{*this};
  --(*this);
  return tmp;
}

// Unary operators
template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> operator+(mat<4, T> m) noexcept {
  return m;
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> operator-(const mat<4, T>& m) noexcept {
  return mat<4, T>{-m[0], -m[1], -m[2], -m[3]};
}

// Scalar binary arithmetic operators
template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T> operator+(mat<4, T> m, U scalar) noexcept {
  m += scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T> operator+(U scalar, mat<4, T> m) noexcept {
  m += scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T> operator-(mat<4, T> m, U scalar) noexcept {
  m -= scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T> operator-(U scalar, const mat<4, T>& m) noexcept {
  mat<4, T> tmp{static_cast<T>(scalar)};
  tmp -= m;
  return tmp;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T> operator*(mat<4, T> m, U scalar) noexcept {
  m *= scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T> operator*(U scalar, mat<4, T> m) noexcept {
  m *= scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T> operator/(mat<4, T> m, U scalar) noexcept {
  m /= scalar;
  return m;
}

template <typename T, Scalar U>
MEN_FORCE_INLINE constexpr mat<4, T> operator/(U scalar, const mat<4, T>& m) noexcept {
  return static_cast<T>(scalar) * detail::inv(m);
}

// Matrix binary arithmetic operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<4, T> operator+(mat<4, T> lhs, const mat<4, U>& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<4, T> operator-(mat<4, T> lhs, const mat<4, U>& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<4, T> operator*(const mat<4, T>& lhs, const mat<4, U>& rhs) noexcept {
  return mat<4, T>{lhs * rhs[0], lhs * rhs[1], lhs * rhs[2], lhs * rhs[3]};
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<4, T> operator/(mat<4, T> lhs, const mat<4, U>& rhs) noexcept {
  lhs *= detail::inv(mat<4, T>{rhs});
  return lhs;
}

// Matrix / vector operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<4, T>::col_t operator*(const mat<4, T>& m, const vec<4, U>& v) noexcept {
  const T x = static_cast<T>(v[0]);
  const T y = static_cast<T>(v[1]);
  const T z = static_cast<T>(v[2]);
  const T w = static_cast<T>(v[3]);
  return (m[0] * x) + (m[1] * y) + (m[2] * z) + (m[3] * w);
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<4, T>::row_t operator*(const vec<4, U>& v, const mat<4, T>& m) noexcept {
  const T x = static_cast<T>(v[0]);
  const T y = static_cast<T>(v[1]);
  const T z = static_cast<T>(v[2]);
  const T w = static_cast<T>(v[3]);

  return typename mat<4, T>::row_t{
      x * m[0][0] + y * m[0][1] + z * m[0][2] + w * m[0][3],
      x * m[1][0] + y * m[1][1] + z * m[1][2] + w * m[1][3],
      x * m[2][0] + y * m[2][1] + z * m[2][2] + w * m[2][3],
      x * m[3][0] + y * m[3][1] + z * m[3][2] + w * m[3][3],
  };
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<4, T>::col_t operator/(const mat<4, T>& m, const vec<4, U>& v) noexcept {
  return detail::inv(m) * vec<4, T>{v};
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr mat<4, T>::row_t operator/(const vec<4, U>& v, const mat<4, T>& m) noexcept {
  return vec<4, T>{v} * detail::inv(m);
}

// Boolean operators
template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator==(const mat<4, T>& m1, const mat<4, U>& m2) noexcept {
  return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]) && (m1[3] == m2[3]);
}

template <typename T, typename U>
MEN_FORCE_INLINE constexpr bool operator!=(const mat<4, T>& m1, const mat<4, U>& m2) noexcept {
  return !(m1 == m2);
}

}  // namespace mEn
