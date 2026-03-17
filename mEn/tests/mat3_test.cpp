#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mEn/fwd.hpp>
#include <mEn/mat3.hpp>

#include "assert/mat3_eq.hpp"
#include "assert/vec3_eq.hpp"
#include "util/test_utils.hpp"

namespace {

template <typename T>
struct Mat3 : ::testing::Test {
  using M = mEn::mat<3, T>;
  using V = mEn::vec<3, T>;
};

using TestedTypes = ::testing::Types<std::int32_t, float, double>;
TYPED_TEST_SUITE(Mat3, TestedTypes);

}  // namespace

TYPED_TEST(Mat3, LengthAndIndexing) {
  using T = TypeParam;
  using M = TestFixture::M;

  M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};
  EXPECT_EQ(m.length(), 3U);

  EXPECT_MAT3_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));

  m[0][0] = T{10};
  m[0][1] = T{20};
  m[0][2] = T{30};
  m[1][0] = T{40};
  m[1][1] = T{50};
  m[1][2] = T{60};
  m[2][0] = T{70};
  m[2][1] = T{80};
  m[2][2] = T{90};

  EXPECT_MAT3_EQ(m, EX(T, 10, 20, 30, 40, 50, 60, 70, 80, 90));

  const M cm{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};
  EXPECT_MAT3_EQ(cm, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TYPED_TEST(Mat3, Constructors) {
  using T = TypeParam;
  using M = TestFixture::M;
  using V = TestFixture::V;
  using U = std::size_t;

  const M id{T{5}};
  EXPECT_MAT3_EQ(id, EX(T, 5, 0, 0, 0, 5, 0, 0, 0, 5));

  const M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};
  EXPECT_MAT3_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));

  const M from_cols{V{T{1}, T{2}, T{3}}, V{T{4}, T{5}, T{6}}, V{T{7}, T{8}, T{9}}};
  EXPECT_TRUE(from_cols == m);

  const M mixed{1, U{2}, 3, 4, U{5}, 6, 7, U{8}, 9};
  EXPECT_MAT3_EQ(mixed, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));

  const mEn::mat<3, U> um{1, 2, 3, 4, 5, 6, 7, 8, 9};
  const M from_u{um};
  EXPECT_TRUE(from_u == m);
}

TYPED_TEST(Mat3, CompoundScalarArithmetic) {
  using T = TypeParam;
  using M = TestFixture::M;
  using U = std::size_t;

  M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};

  m += U{2};
  EXPECT_MAT3_EQ(m, EX(T, 3, 4, 5, 6, 7, 8, 9, 10, 11));

  m -= U{2};
  EXPECT_MAT3_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));

  m *= U{2};
  EXPECT_MAT3_EQ(m, EX(T, 2, 4, 6, 8, 10, 12, 14, 16, 18));

  m /= U{2};
  EXPECT_MAT3_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TYPED_TEST(Mat3, CompoundMatrixArithmeticWithMixedTypes) {
  using T = TypeParam;
  using M = TestFixture::M;
  using U = std::size_t;

  M a{T{1}};  // identity
  const mEn::mat<3, U> b{1, 2, 3, 4, 5, 6, 7, 8, 9};

  a += b;
  EXPECT_MAT3_EQ(a, EX(T, 2, 2, 3, 4, 6, 6, 7, 8, 10));

  a -= b;
  EXPECT_MAT3_EQ(a, EX(T, 1, 0, 0, 0, 1, 0, 0, 0, 1));

  a *= b;  // identity * b = b
  EXPECT_MAT3_EQ(a, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));

  a = b;
  EXPECT_MAT3_EQ(a, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TYPED_TEST(Mat3, IncrementDecrement) {
  using T = TypeParam;
  using M = TestFixture::M;

  M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};

  const M post_inc = m++;
  EXPECT_MAT3_EQ(post_inc, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));
  EXPECT_MAT3_EQ(m, EX(T, 2, 3, 4, 5, 6, 7, 8, 9, 10));

  const M pre_inc = ++m;
  EXPECT_MAT3_EQ(pre_inc, EX(T, 3, 4, 5, 6, 7, 8, 9, 10, 11));
  EXPECT_MAT3_EQ(m, EX(T, 3, 4, 5, 6, 7, 8, 9, 10, 11));

  const M post_dec = m--;
  EXPECT_MAT3_EQ(post_dec, EX(T, 3, 4, 5, 6, 7, 8, 9, 10, 11));
  EXPECT_MAT3_EQ(m, EX(T, 2, 3, 4, 5, 6, 7, 8, 9, 10));

  const M pre_dec = --m;
  EXPECT_MAT3_EQ(pre_dec, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));
  EXPECT_MAT3_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TYPED_TEST(Mat3, UnaryOperators) {
  using T = TypeParam;
  using M = TestFixture::M;

  const M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};

  const M plus = +m;
  EXPECT_TRUE(plus == m);

  const M neg = -m;
  EXPECT_MAT3_EQ(neg, EX(T, -1, -2, -3, -4, -5, -6, -7, -8, -9));
}

TYPED_TEST(Mat3, BinaryScalarOperators) {
  using T = TypeParam;
  using M = TestFixture::M;

  const M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};

  EXPECT_MAT3_EQ(m + T{1}, EX(T, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  EXPECT_MAT3_EQ(T{1} + m, EX(T, 2, 3, 4, 5, 6, 7, 8, 9, 10));

  EXPECT_MAT3_EQ(m - T{1}, EX(T, 0, 1, 2, 3, 4, 5, 6, 7, 8));

  // scalar - matrix is defined as (scalar*I) - m
  const M s_minus = T{5} - m;
  EXPECT_MAT3_EQ(s_minus, EX(T, 4, -2, -3, -4, 0, -6, -7, -8, -4));

  EXPECT_MAT3_EQ(m * T{2}, EX(T, 2, 4, 6, 8, 10, 12, 14, 16, 18));
  EXPECT_MAT3_EQ(T{2} * m, EX(T, 2, 4, 6, 8, 10, 12, 14, 16, 18));

  if constexpr (std::is_floating_point_v<T>) {
    EXPECT_MAT3_EQ(m / T{2}, EX(T, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5));
  } else {
    EXPECT_MAT3_EQ(m / T{2}, EX(T, 0, 1, 1, 2, 2, 3, 3, 4, 4));
  }
}

TYPED_TEST(Mat3, BinaryMatrixOperators) {
  using T = TypeParam;
  using M = TestFixture::M;

  const M a{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};
  const M b{T{9}, T{8}, T{7}, T{6}, T{5}, T{4}, T{3}, T{2}, T{1}};

  EXPECT_MAT3_EQ(a + b, EX(T, 10, 10, 10, 10, 10, 10, 10, 10, 10));

  EXPECT_MAT3_EQ(a - b, EX(T, -8, -6, -4, -2, 0, 2, 4, 6, 8));

  // Multiplication sanity: A * D scales columns of A by diagonal entries of D.
  const M d{T{2}, T{0}, T{0}, T{0}, T{3}, T{0}, T{0}, T{0}, T{4}};

  const M a_times_d = a * d;
  EXPECT_MAT3_EQ(a_times_d, EX(T, 2, 4, 6, 12, 15, 18, 28, 32, 36));
}

TYPED_TEST(Mat3, MatrixVectorOperators) {
  using T = TypeParam;
  using M = TestFixture::M;
  using V = TestFixture::V;

  const M d{T{2}, T{0}, T{0}, T{0}, T{3}, T{0}, T{0}, T{0}, T{4}};

  const V v{T{1}, T{1}, T{1}};

  const V mv = d * v;
  EXPECT_VEC3_EQ(mv, EX(T, 2, 3, 4));

  const V vm = v * d;
  EXPECT_VEC3_EQ(vm, EX(T, 2, 3, 4));

  if constexpr (std::is_floating_point_v<T>) {
    const V u{T{1}, T{2}, T{4}};
    const V div_left  = d / u;  // inverse(d) * u
    const V div_right = u / d;  // u * inverse(d)

    EXPECT_VEC3_EQ(div_left, EX(T, 0.5, (T{2} / T{3}), 1));
    EXPECT_VEC3_EQ(div_right, EX(T, 0.5, (T{2} / T{3}), 1));
  }
}

TYPED_TEST(Mat3, EqualityAndInequality) {
  using T = TypeParam;
  using M = TestFixture::M;

  const M a{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};
  const M b{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  const M c{T{1}, T{2}, T{3}, T{4}, T{999}, T{6}, T{7}, T{8}, T{9}};
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
}

TYPED_TEST(Mat3, FloatingEqualityUsesEpsilon) {
  using T = TypeParam;
  using M = TestFixture::M;

  if constexpr (!std::is_floating_point_v<T>) {
    GTEST_SKIP() << "epsilon equality applies only to floating-point types";
  } else {
    const M a{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};
    const auto half_eps = T{0.5} * mEn::kEpsilon<T>;

    const M within{
        T{1} + half_eps, T{2} - half_eps, T{3} + half_eps, T{4} - half_eps, T{5} + half_eps,
        T{6} - half_eps, T{7} + half_eps, T{8} - half_eps, T{9} + half_eps,
    };
    EXPECT_TRUE(a == within);

    const M outside{
        T{1} + (T{2} * mEn::kEpsilon<T>), T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9},
    };
    EXPECT_FALSE(a == outside);
  }
}

TYPED_TEST(Mat3, FloatingDivisionByMatrix) {
  using T = TypeParam;
  using M = TestFixture::M;

  if constexpr (!std::is_floating_point_v<T>) {
    GTEST_SKIP() << "matrix division relies on inverse; tested only for floating-point types";
  } else {
    const M d{T{2}, T{0}, T{0}, T{0}, T{3}, T{0}, T{0}, T{0}, T{4}};
    const M id{T{1}};

    EXPECT_TRUE((d / d) == id);

    const M inv = id / d;
    EXPECT_MAT3_EQ(inv, EX(T, 0.5, 0, 0, 0, (T{1} / T{3}), 0, 0, 0, 0.25));

    const M scalar_over = T{2} / id;
    EXPECT_MAT3_EQ(scalar_over, EX(T, 2, 0, 0, 0, 2, 0, 0, 0, 2));
  }
}
