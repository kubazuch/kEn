#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mEn/fwd.hpp>
#include <mEn/mat4.hpp>

#include "assert/mat4_eq.hpp"
#include "assert/vec4_eq.hpp"
#include "util/test_utils.hpp"

namespace {

template <typename T>
struct Mat4 : ::testing::Test {
  using M = mEn::mat<4, T>;
  using V = mEn::vec<4, T>;
};

using TestedTypes = ::testing::Types<std::int32_t, float, double>;
TYPED_TEST_SUITE(Mat4, TestedTypes);

}  // namespace

TYPED_TEST(Mat4, LengthAndIndexing) {
  // NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
  using T = TypeParam;
  using M = TestFixture::M;

  M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
  EXPECT_EQ(m.length(), 4U);

  EXPECT_MAT4_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

  m[0][0] = T{10};
  m[0][1] = T{20};
  m[0][2] = T{30};
  m[0][3] = T{40};

  m[1][0] = T{50};
  m[1][1] = T{60};
  m[1][2] = T{70};
  m[1][3] = T{80};

  m[2][0] = T{90};
  m[2][1] = T{100};
  m[2][2] = T{110};
  m[2][3] = T{120};

  m[3][0] = T{130};
  m[3][1] = T{140};
  m[3][2] = T{150};
  m[3][3] = T{160};

  EXPECT_MAT4_EQ(m, EX(T, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160));

  const M cm{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
  EXPECT_MAT4_EQ(cm, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
  // NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}

TYPED_TEST(Mat4, Constructors) {
  using T = TypeParam;
  using M = TestFixture::M;
  using V = TestFixture::V;
  using U = size_t;

  const M id{T{5}};
  EXPECT_MAT4_EQ(id, EX(T, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5));

  const M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
  EXPECT_MAT4_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

  const M from_cols{V{T{1}, T{2}, T{3}, T{4}}, V{T{5}, T{6}, T{7}, T{8}}, V{T{9}, T{10}, T{11}, T{12}},
                    V{T{13}, T{14}, T{15}, T{16}}};
  EXPECT_TRUE(from_cols == m);

  const M mixed{1, U{2}, 3, 4, 5, U{6}, 7, 8, 9, U{10}, 11, 12, 13, U{14}, 15, 16};
  EXPECT_MAT4_EQ(mixed, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

  const mEn::mat<4, U> um{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  const M from_u{um};
  EXPECT_TRUE(from_u == m);
}

TYPED_TEST(Mat4, CompoundScalarArithmetic) {
  using T = TypeParam;
  using M = TestFixture::M;
  using U = size_t;

  M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};

  m += U{2};
  EXPECT_MAT4_EQ(m, EX(T, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18));

  m -= U{2};
  EXPECT_MAT4_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

  m *= U{2};
  EXPECT_MAT4_EQ(m, EX(T, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));

  m /= U{2};
  EXPECT_MAT4_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}

TYPED_TEST(Mat4, CompoundMatrixArithmeticWithMixedTypes) {
  using T = TypeParam;
  using M = TestFixture::M;
  using U = size_t;

  M a{T{1}};
  const mEn::mat<4, U> b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

  a += b;
  EXPECT_MAT4_EQ(a, EX(T, 2, 2, 3, 4, 5, 7, 7, 8, 9, 10, 12, 12, 13, 14, 15, 17));

  a -= b;
  EXPECT_MAT4_EQ(a, EX(T, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

  a *= b;
  EXPECT_MAT4_EQ(a, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

  a = b;
  EXPECT_MAT4_EQ(a, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}

TYPED_TEST(Mat4, IncrementDecrement) {
  using T = TypeParam;
  using M = TestFixture::M;

  M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};

  const M post_inc = m++;
  EXPECT_MAT4_EQ(post_inc, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
  EXPECT_MAT4_EQ(m, EX(T, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17));

  const M pre_inc = ++m;
  EXPECT_MAT4_EQ(pre_inc, EX(T, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18));
  EXPECT_MAT4_EQ(m, EX(T, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18));

  const M post_dec = m--;
  EXPECT_MAT4_EQ(post_dec, EX(T, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18));
  EXPECT_MAT4_EQ(m, EX(T, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17));

  const M pre_dec = --m;
  EXPECT_MAT4_EQ(pre_dec, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
  EXPECT_MAT4_EQ(m, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}

TYPED_TEST(Mat4, UnaryOperators) {
  using T = TypeParam;
  using M = TestFixture::M;

  const M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};

  const M plus = +m;
  EXPECT_TRUE(plus == m);

  const M neg = -m;
  EXPECT_MAT4_EQ(neg, EX(T, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16));
}

TYPED_TEST(Mat4, BinaryScalarOperators) {
  using T = TypeParam;
  using M = TestFixture::M;

  const M m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};

  EXPECT_MAT4_EQ(m + T{1}, EX(T, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17));
  EXPECT_MAT4_EQ(T{1} + m, EX(T, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17));

  EXPECT_MAT4_EQ(m - T{1}, EX(T, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));

  const M s_minus = T{5} - m;
  EXPECT_MAT4_EQ(s_minus, EX(T, 4, -2, -3, -4, -5, -1, -7, -8, -9, -10, -6, -12, -13, -14, -15, -11));

  EXPECT_MAT4_EQ(m * T{2}, EX(T, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));
  EXPECT_MAT4_EQ(T{2} * m, EX(T, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));

  if constexpr (std::is_floating_point_v<T>) {
    EXPECT_MAT4_EQ(m / T{2}, EX(T, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8));
  } else {
    EXPECT_MAT4_EQ(m / T{2}, EX(T, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8));
  }
}

TYPED_TEST(Mat4, BinaryMatrixOperators) {
  using T = TypeParam;
  using M = TestFixture::M;

  const M a{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
  const M b{T{16}, T{15}, T{14}, T{13}, T{12}, T{11}, T{10}, T{9}, T{8}, T{7}, T{6}, T{5}, T{4}, T{3}, T{2}, T{1}};

  EXPECT_MAT4_EQ(a + b, EX(T, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17));

  EXPECT_MAT4_EQ(a - b, EX(T, -15, -13, -11, -9, -7, -5, -3, -1, 1, 3, 5, 7, 9, 11, 13, 15));

  const M d{T{2}, T{0}, T{0}, T{0}, T{0}, T{3}, T{0}, T{0}, T{0}, T{0}, T{4}, T{0}, T{0}, T{0}, T{0}, T{5}};

  const M a_times_d = a * d;
  EXPECT_MAT4_EQ(a_times_d, EX(T, 2, 4, 6, 8, 15, 18, 21, 24, 36, 40, 44, 48, 65, 70, 75, 80));
}

TYPED_TEST(Mat4, MatrixVectorOperators) {
  using T = TypeParam;
  using M = TestFixture::M;
  using V = TestFixture::V;

  const M d{T{2}, T{0}, T{0}, T{0}, T{0}, T{3}, T{0}, T{0}, T{0}, T{0}, T{4}, T{0}, T{0}, T{0}, T{0}, T{5}};

  const V v{T{1}, T{1}, T{1}, T{1}};

  const V mv = d * v;
  EXPECT_VEC4_EQ(mv, EX(T, 2, 3, 4, 5));

  const V vm = v * d;
  EXPECT_VEC4_EQ(vm, EX(T, 2, 3, 4, 5));

  if constexpr (std::is_floating_point_v<T>) {
    const V u{T{1}, T{2}, T{4}, T{10}};
    const V div_left  = d / u;  // inverse(d) * u
    const V div_right = u / d;  // u * inverse(d)

    EXPECT_VEC4_EQ(div_left, EX(T, 0.5, (T{2} / T{3}), 1, 2));
    EXPECT_VEC4_EQ(div_right, EX(T, 0.5, (T{2} / T{3}), 1, 2));
  }
}

TYPED_TEST(Mat4, EqualityAndInequality) {
  using T = TypeParam;
  using M = TestFixture::M;

  const M a{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
  const M b{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  const M c{T{1}, T{2}, T{3}, T{4}, T{5}, T{999}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
}

TYPED_TEST(Mat4, FloatingEqualityUsesEpsilon) {
  using T = TypeParam;
  using M = TestFixture::M;

  if constexpr (!std::is_floating_point_v<T>) {
    GTEST_SKIP() << "epsilon equality applies only to floating-point types";
  } else {
    const M a{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
    const auto half_eps = T{0.5} * mEn::kEpsilon<T>;

    const M within{
        T{1} + half_eps,  T{2} - half_eps,  T{3} + half_eps,  T{4} - half_eps,  T{5} + half_eps,  T{6} - half_eps,
        T{7} + half_eps,  T{8} - half_eps,  T{9} + half_eps,  T{10} - half_eps, T{11} + half_eps, T{12} - half_eps,
        T{13} + half_eps, T{14} - half_eps, T{15} + half_eps, T{16} - half_eps,
    };
    EXPECT_TRUE(a == within);

    const M outside{T{1} + (T{2} * mEn::kEpsilon<T>),
                    T{2},
                    T{3},
                    T{4},
                    T{5},
                    T{6},
                    T{7},
                    T{8},
                    T{9},
                    T{10},
                    T{11},
                    T{12},
                    T{13},
                    T{14},
                    T{15},
                    T{16}};
    EXPECT_FALSE(a == outside);
  }
}

TYPED_TEST(Mat4, FloatingDivisionByMatrix) {
  using T = TypeParam;
  using M = TestFixture::M;

  if constexpr (!std::is_floating_point_v<T>) {
    GTEST_SKIP() << "matrix division relies on inverse; tested only for floating-point types";
  } else {
    const M d{T{2}, T{0}, T{0}, T{0}, T{0}, T{3}, T{0}, T{0}, T{0}, T{0}, T{4}, T{0}, T{0}, T{0}, T{0}, T{5}};
    const M id{T{1}};

    EXPECT_TRUE((d / d) == id);

    const M inv = id / d;
    EXPECT_MAT4_EQ(inv, EX(T, 0.5, 0, 0, 0, 0, (T{1} / T{3}), 0, 0, 0, 0, 0.25, 0, 0, 0, 0, 0.2));

    const M scalar_over = T{2} / id;
    EXPECT_MAT4_EQ(scalar_over, EX(T, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2));
  }
}
