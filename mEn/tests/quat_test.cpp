#include <gtest/gtest.h>

#include <numbers>
#include <type_traits>

#include <mEn/constants.hpp>
#include <mEn/fwd.hpp>
#include <mEn/quat.hpp>

#include "assert/mat3_eq.hpp"
#include "assert/mat4_eq.hpp"
#include "assert/quat_eq.hpp"
#include "assert/vec3_eq.hpp"
#include "assert/vec4_eq.hpp"
#include "util/test_utils.hpp"

namespace {

template <class T>
struct Quat : ::testing::Test {
  using Q = mEn::qua<T>;
};

template <class T>
using OtherT = std::conditional_t<std::is_same_v<T, double>, float, double>;

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Quat, TestedTypes);

}  // namespace

TYPED_TEST(Quat, LengthAndIndexing) {
  // NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
  using T = TypeParam;
  using Q = TestFixture::Q;

  Q q{T{1}, T{2}, T{3}, T{4}};
  EXPECT_EQ(Q::length(), 4U);
  EXPECT_EQ(q.length(), 4U);

  EXPECT_EQ(q[0], q.x);
  EXPECT_EQ(q[1], q.y);
  EXPECT_EQ(q[2], q.z);
  EXPECT_EQ(q[3], q.w);

  q[0] = T{10};
  q[1] = T{20};
  q[2] = T{30};
  q[3] = T{40};
  EXPECT_QUAT_EQ(q, EX(T, 10, 20, 30, 40));

  const Q cq{T{4}, T{5}, T{6}, T{7}};
  EXPECT_EQ(cq[0], cq.x);
  EXPECT_EQ(cq[1], cq.y);
  EXPECT_EQ(cq[2], cq.z);
  EXPECT_EQ(cq[3], cq.w);
  // NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}

TYPED_TEST(Quat, ConstructorWXYZ) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const Q q{T{1}, T{2}, T{3}, T{4}};
  EXPECT_QUAT_EQ(q, EX(T, 2, 3, 4, 1));
}

TYPED_TEST(Quat, ConstructorScalarVec3) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const mEn::vec<3, T> v{T{2}, T{3}, T{4}};
  const Q q{T{1}, v};
  EXPECT_QUAT_EQ(q, EX(T, 2, 3, 4, 1));
}

TYPED_TEST(Quat, ConstructorConversionExplicit) {
  using T  = TypeParam;
  using Q  = TestFixture::Q;
  using U  = OtherT<T>;
  using QU = mEn::qua<U>;

  static_assert(std::is_constructible_v<Q, const QU&>);
  static_assert(!std::is_convertible_v<QU, Q>);

  const QU qu{U{1}, U{2}, U{3}, U{4}};
  const Q q{qu};
  EXPECT_QUAT_EQ(q, EX(T, 2, 3, 4, 1));
}

TYPED_TEST(Quat, ExplicitConstructorsAndConversionsStayExplicit) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  static_assert(!std::is_convertible_v<mEn::vec<3, T>, Q>);
  static_assert(!std::is_convertible_v<mEn::mat<3, T>, Q>);
  static_assert(!std::is_convertible_v<mEn::mat<4, T>, Q>);
  static_assert(!std::is_convertible_v<Q, mEn::mat<3, T>>);
  static_assert(!std::is_convertible_v<Q, mEn::mat<4, T>>);

  SUCCEED();
}

TYPED_TEST(Quat, AssignmentFromOtherQuaternionType) {
  using T  = TypeParam;
  using Q  = TestFixture::Q;
  using U  = OtherT<T>;
  using QU = mEn::qua<U>;

  Q q{T{0}, T{0}, T{0}, T{0}};
  const QU src{U{1.5}, U{2.5}, U{3.5}, U{4.5}};

  EXPECT_EQ(&(q = src), &q);
  EXPECT_QUAT_EQ(q, EX(T, 2.5, 3.5, 4.5, 1.5));
}

TYPED_TEST(Quat, ConstructorFromTwoVectors) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  // Rotation from (1,0,0) to (0,1,0) is 90deg around Z.
  // q * (1,0,0) should give (0,1,0).
  const mEn::vec<3, T> u{T{1}, T{0}, T{0}};
  const mEn::vec<3, T> v{T{0}, T{1}, T{0}};
  const Q q{u, v};
  const auto result = q * u;
  EXPECT_VEC3_EQ(result, EX(T, 0, 1, 0));
}

TYPED_TEST(Quat, ConstructorFromTwoOppositeVectors) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const mEn::vec<3, T> u{T{1}, T{0}, T{0}};
  const mEn::vec<3, T> v{T{-1}, T{0}, T{0}};
  const Q q{u, v};

  EXPECT_VEC3_EQ(q * u, EX(T, -1, 0, 0));
}

TYPED_TEST(Quat, ConstructorFromEulerAngles) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  // 90deg rotation around Z: euler = (0, 0, pi/2)
  // Expected: w = cos(pi/4) = sqrt(2)/2, x=0, y=0, z = sin(pi/4) = sqrt(2)/2
  const T half_sqrt2 = static_cast<T>(std::numbers::sqrt2 / 2.0);
  const mEn::vec<3, T> euler{T{0}, T{0}, static_cast<T>(std::numbers::pi) / T{2}};
  const Q q{euler};
  EXPECT_QUAT_EQ(q, EX(T, 0, 0, half_sqrt2, half_sqrt2));

  const mEn::vec<3, T> rotated = q * mEn::vec<3, T>{T{1}, T{0}, T{0}};
  EXPECT_VEC3_EQ(rotated, EX(T, 0, 1, 0));
}

TYPED_TEST(Quat, ConversionToMat3) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const Q identity{T{1}, T{0}, T{0}, T{0}};
  const auto m = static_cast<mEn::mat<3, T>>(identity);
  EXPECT_MAT3_EQ(m, EX(T, 1, 0, 0, 0, 1, 0, 0, 0, 1));

  // 90-deg rotation around Z:
  //   col 0: (0, 1, 0),  col 1: (-1, 0, 0),  col 2: (0, 0, 1)
  const mEn::vec<3, T> euler{T{0}, T{0}, static_cast<T>(std::numbers::pi) / T{2}};
  const auto m_z90 = static_cast<mEn::mat<3, T>>(Q{euler});
  EXPECT_MAT3_EQ(m_z90, EX(T, 0, 1, 0, -1, 0, 0, 0, 0, 1));
}

TYPED_TEST(Quat, ConversionToMat4) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const Q identity{T{1}, T{0}, T{0}, T{0}};
  const auto m = static_cast<mEn::mat<4, T>>(identity);
  EXPECT_MAT4_EQ(m, EX(T, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

  // 90-deg rotation around Z embedded in mat4
  const mEn::vec<3, T> euler{T{0}, T{0}, static_cast<T>(std::numbers::pi) / T{2}};
  const auto m_z90 = static_cast<mEn::mat<4, T>>(Q{euler});
  EXPECT_MAT4_EQ(m_z90, EX(T, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
}

TYPED_TEST(Quat, ConstructorFromMat3) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const mEn::mat<3, T> identity_m{T{1}};
  const Q q_identity{identity_m};
  EXPECT_QUAT_EQ(q_identity, EX(T, 0, 0, 0, 1));

  // 90-deg rotation around Z: mat -> quat, verify q * (1,0,0) = (0,1,0)
  const mEn::mat<3, T> m_z90{T{0}, T{1}, T{0}, T{-1}, T{0}, T{0}, T{0}, T{0}, T{1}};
  const Q q{m_z90};
  EXPECT_VEC3_EQ((q * mEn::vec<3, T>{T{1}, T{0}, T{0}}), EX(T, 0, 1, 0));
}

TYPED_TEST(Quat, ConstructorFromMat3CoversDominantBranches) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const T pi = static_cast<T>(std::numbers::pi);

  {
    const Q qx{mEn::vec<3, T>{pi, T{0}, T{0}}};
    const Q rx{static_cast<mEn::mat<3, T>>(qx)};
    EXPECT_VEC3_EQ((rx * mEn::vec<3, T>{T{0}, T{1}, T{0}}), EX(T, 0, -1, 0));
  }

  {
    const Q qy{mEn::vec<3, T>{T{0}, pi, T{0}}};
    const Q ry{static_cast<mEn::mat<3, T>>(qy)};
    EXPECT_VEC3_EQ((ry * mEn::vec<3, T>{T{1}, T{0}, T{0}}), EX(T, -1, 0, 0));
  }

  {
    const Q qz{mEn::vec<3, T>{T{0}, T{0}, pi}};
    const Q rz{static_cast<mEn::mat<3, T>>(qz)};
    EXPECT_VEC3_EQ((rz * mEn::vec<3, T>{T{1}, T{0}, T{0}}), EX(T, -1, 0, 0));
  }
}

TYPED_TEST(Quat, ConstructorFromMat4) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const mEn::mat<4, T> identity_m{T{1}};
  const Q q_identity{identity_m};
  EXPECT_QUAT_EQ(q_identity, EX(T, 0, 0, 0, 1));

  // 90-deg rotation around Z embedded in mat4 -> quat
  const mEn::mat<4, T> m_z90{T{0}, T{1}, T{0}, T{0}, T{-1}, T{0}, T{0}, T{0},
                             T{0}, T{0}, T{1}, T{0}, T{0},  T{0}, T{0}, T{1}};
  const Q q{m_z90};
  EXPECT_VEC3_EQ((q * mEn::vec<3, T>{T{1}, T{0}, T{0}}), EX(T, 0, 1, 0));
}

TYPED_TEST(Quat, UnaryOperators) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const Q q{T{1}, T{2}, T{3}, T{4}};

  const Q pos = +q;
  EXPECT_QUAT_EQ(pos, EX(T, 2, 3, 4, 1));

  const Q neg = -q;
  EXPECT_QUAT_EQ(neg, EX(T, -2, -3, -4, -1));
}

TYPED_TEST(Quat, CompoundScalarArithmetic) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  Q q{T{1}, T{2}, T{4}, T{8}};

  EXPECT_EQ(&(q *= 2), &q);
  EXPECT_QUAT_EQ(q, EX(T, 4, 8, 16, 2));

  EXPECT_EQ(&(q /= 4.0), &q);
  EXPECT_QUAT_EQ(q, EX(T, 1, 2, 4, 0.5));
}

TYPED_TEST(Quat, CompoundQuatArithmetic) {
  using T  = TypeParam;
  using Q  = TestFixture::Q;
  using U  = OtherT<T>;
  using QU = mEn::qua<U>;

  Q a{T{1}, T{2}, T{3}, T{4}};
  const QU b{U{0}, U{1}, U{1}, U{1}};

  EXPECT_EQ(&(a += b), &a);
  EXPECT_QUAT_EQ(a, EX(T, 3, 4, 5, 1));

  EXPECT_EQ(&(a -= b), &a);
  EXPECT_QUAT_EQ(a, EX(T, 2, 3, 4, 1));

  // i * j = k
  Q i{T{0}, T{1}, T{0}, T{0}};
  const QU j{U{0}, U{0}, U{1}, U{0}};
  EXPECT_EQ(&(i *= j), &i);
  EXPECT_QUAT_EQ(i, EX(T, 0, 0, 1, 0));
}

TYPED_TEST(Quat, ScalarBinaryOperators) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const Q q{T{1}, T{2}, T{3}, T{4}};

  EXPECT_QUAT_EQ(q * 2, EX(T, 4, 6, 8, 2));
  EXPECT_QUAT_EQ(2 * q, EX(T, 4, 6, 8, 2));
  EXPECT_QUAT_EQ(q / 2.0, EX(T, 1, 1.5, 2, 0.5));
}

TYPED_TEST(Quat, QuatBinaryOperators) {
  using T  = TypeParam;
  using Q  = TestFixture::Q;
  using U  = OtherT<T>;
  using QU = mEn::qua<U>;

  const Q a{T{1}, T{2}, T{3}, T{4}};
  const QU b{U{0}, U{1}, U{1}, U{1}};

  EXPECT_QUAT_EQ(a + b, EX(T, 3, 4, 5, 1));
  EXPECT_QUAT_EQ(a - b, EX(T, 1, 2, 3, 1));

  const Q qi{T{0}, T{1}, T{0}, T{0}};
  const QU qj{U{0}, U{0}, U{1}, U{0}};
  EXPECT_QUAT_EQ(qi * qj, EX(T, 0, 0, 1, 0));
}

TYPED_TEST(Quat, VectorMultiplicationWithIdentity) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const Q identity{T{1}, T{0}, T{0}, T{0}};
  const mEn::vec<3, T> v3{T{1}, T{2}, T{3}};
  const mEn::vec<4, T> v4{T{1}, T{2}, T{3}, T{4}};

  const auto r3 = identity * v3;
  EXPECT_VEC3_EQ(r3, EX(T, 1, 2, 3));

  const auto r3_rev = v3 * identity;
  EXPECT_VEC3_EQ(r3_rev, EX(T, 1, 2, 3));

  const auto r4 = identity * v4;
  EXPECT_VEC4_EQ(r4, EX(T, 1, 2, 3, 4));

  const auto r4_rev = v4 * identity;
  EXPECT_VEC4_EQ(r4_rev, EX(T, 1, 2, 3, 4));
}

TYPED_TEST(Quat, ReverseOrderVectorMultiplicationUsesInverseRotation) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const Q q{mEn::vec<3, T>{T{0}, T{0}, mEn::kHalfPi<T>}};

  const mEn::vec<3, T> v3{T{1}, T{0}, T{0}};
  EXPECT_VEC3_EQ(q * v3, EX(T, 0, 1, 0));
  EXPECT_VEC3_EQ(v3 * q, EX(T, 0, -1, 0));

  const mEn::vec<4, T> v4{T{1}, T{0}, T{0}, T{7}};
  EXPECT_VEC4_EQ(q * v4, EX(T, 0, 1, 0, 7));
  EXPECT_VEC4_EQ(v4 * q, EX(T, 0, -1, 0, 7));
}

TYPED_TEST(Quat, EqualityAndInequality) {
  using T  = TypeParam;
  using Q  = TestFixture::Q;
  using U  = OtherT<T>;
  using QU = mEn::qua<U>;

  const Q a{T{1}, T{2}, T{3}, T{4}};
  const QU b{U{1}, U{2}, U{3}, U{4}};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  const QU c{U{1}, U{3}, U{3}, U{4}};
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
}

TYPED_TEST(Quat, FloatingEqualityUsesEpsilon) {
  using T = TypeParam;
  using Q = TestFixture::Q;

  const Q a{T{1}, T{2}, T{3}, T{4}};
  const auto half_eps = T{0.5} * mEn::kEpsilon<T>;

  const Q within{
      T{1} + half_eps,
      T{2} - half_eps,
      T{3} + half_eps,
      T{4} - half_eps,
  };
  EXPECT_TRUE(a == within);

  const Q outside{
      T{1} + (T{2} * mEn::kEpsilon<T>),
      T{2},
      T{3},
      T{4},
  };
  EXPECT_FALSE(a == outside);
}
