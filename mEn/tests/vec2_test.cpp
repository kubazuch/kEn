#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mEn/constants.hpp>
#include <mEn/vec2.hpp>

#include "assert/vec2_eq.hpp"
#include "util/test_utils.hpp"

namespace {

template <typename T>
struct Vec2 : ::testing::Test {
  using V = mEn::vec<2, T>;
};

using TestedTypes = ::testing::Types<std::int32_t, float, double>;
TYPED_TEST_SUITE(Vec2, TestedTypes);

}  // namespace

TYPED_TEST(Vec2, LengthAndIndexing) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  V v{T{1}, T{2}};
  EXPECT_EQ(v.length(), 2U);

  EXPECT_EQ(v[0], v.x);
  EXPECT_EQ(v[1], v.y);

  v[0] = T{10};
  v[1] = T{20};
  EXPECT_VEC2_EQ(v, (EX(T, 10, 20)));

  const V cv{T{3}, T{4}};
  EXPECT_EQ(cv[0], cv.x);
  EXPECT_EQ(cv[1], cv.y);
}

TYPED_TEST(Vec2, AliasesShareStorage) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  V v{T{1}, T{2}};

  v.x = T{3};
  EXPECT_EQ(v.r, T{3});
  EXPECT_EQ(v.s, T{3});

  v.r = T{5};
  EXPECT_EQ(v.x, T{5});
  EXPECT_EQ(v.s, T{5});

  v.s = T{7};
  EXPECT_EQ(v.x, T{7});
  EXPECT_EQ(v.r, T{7});

  v.y = T{9};
  EXPECT_EQ(v.g, T{9});
  EXPECT_EQ(v.t, T{9});

  v.g = T{11};
  EXPECT_EQ(v.y, T{11});
  EXPECT_EQ(v.t, T{11});

  v.t = T{13};
  EXPECT_EQ(v.y, T{13});
  EXPECT_EQ(v.g, T{13});
}

TYPED_TEST(Vec2, Constructors) {
  using T = TypeParam;
  using V = typename TestFixture::V;
  using U = std::size_t;

  V splat{T{5}};
  EXPECT_VEC2_EQ(splat, EX(T, 5, 5));

  V pair{T{6}, T{7}};
  EXPECT_VEC2_EQ(pair, EX(T, 6, 7));

  V mixed{1, U{2}};
  EXPECT_VEC2_EQ(mixed, EX(T, 1, 2));

  mEn::vec<2, U> u{U{1}, U{2}};
  V from_u{u};
  EXPECT_VEC2_EQ(from_u, EX(T, 1, 2));
}

TYPED_TEST(Vec2, CompoundScalarArithmetic) {
  using T = TypeParam;
  using V = typename TestFixture::V;
  using U = std::size_t;

  V v{T{1}, T{2}};

  v += U{2};
  EXPECT_VEC2_EQ(v, EX(T, 3, 4));

  v -= U{1};
  EXPECT_VEC2_EQ(v, EX(T, 2, 3));

  v *= U{3};
  EXPECT_VEC2_EQ(v, EX(T, 6, 9));

  v /= U{3};
  EXPECT_VEC2_EQ(v, EX(T, 2, 3));
}

TYPED_TEST(Vec2, CompoundVectorArithmeticWithMixedTypes) {
  using T = TypeParam;
  using V = typename TestFixture::V;
  using U = std::size_t;

  V v{T{1}, T{2}};
  mEn::vec<2, U> i{2, 3};

  v += i;
  EXPECT_VEC2_EQ(v, EX(T, 3, 5));

  v -= i;
  EXPECT_VEC2_EQ(v, EX(T, 1, 2));

  v *= i;
  EXPECT_VEC2_EQ(v, EX(T, 2, 6));

  v /= i;
  EXPECT_VEC2_EQ(v, EX(T, 1, 2));

  v = i;
  EXPECT_VEC2_EQ(v, EX(T, 2, 3));
}

TYPED_TEST(Vec2, IncrementDecrement) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  V v{T{1}, T{2}};

  V post_inc = v++;
  EXPECT_VEC2_EQ(post_inc, EX(T, 1, 2));
  EXPECT_VEC2_EQ(v, EX(T, 2, 3));

  V pre_inc = ++v;
  EXPECT_VEC2_EQ(pre_inc, EX(T, 3, 4));
  EXPECT_VEC2_EQ(v, EX(T, 3, 4));

  V post_dec = v--;
  EXPECT_VEC2_EQ(post_dec, EX(T, 3, 4));
  EXPECT_VEC2_EQ(v, EX(T, 2, 3));

  V pre_dec = --v;
  EXPECT_VEC2_EQ(pre_dec, EX(T, 1, 2));
  EXPECT_VEC2_EQ(v, EX(T, 1, 2));
}

TYPED_TEST(Vec2, UnaryOperators) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  V v{T{2}, T{3}};

  V plus = +v;
  EXPECT_VEC2_EQ(plus, EX(T, 2, 3));

  V neg = -v;
  EXPECT_VEC2_EQ(neg, EX(T, -2, -3));
}

TYPED_TEST(Vec2, BinaryScalarOperators) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  const V v{T{1}, T{2}};

  EXPECT_VEC2_EQ(v + T{2}, EX(T, 3, 4));
  EXPECT_VEC2_EQ(T{2} + v, EX(T, 3, 4));

  EXPECT_VEC2_EQ(v - T{2}, EX(T, -1, 0));
  EXPECT_VEC2_EQ(T{5} - v, EX(T, 4, 3));

  EXPECT_VEC2_EQ(v * T{3}, EX(T, 3, 6));
  EXPECT_VEC2_EQ(T{3} * v, EX(T, 3, 6));

  if constexpr (std::is_floating_point_v<T>) {
    EXPECT_VEC2_EQ(v / T{2}, EX(T, 0.5, 1));
  } else {
    EXPECT_VEC2_EQ(v / T{2}, EX(T, 0, 1));
  }
  EXPECT_VEC2_EQ((T{6} / V{T{2}, T{3}}), EX(T, 3, 2));
}

TYPED_TEST(Vec2, BinaryVectorOperators) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  const V a{T{1}, T{2}};
  const V b{T{3}, T{5}};

  EXPECT_VEC2_EQ(a + b, EX(T, 4, 7));
  EXPECT_VEC2_EQ(a - b, EX(T, -2, -3));
  EXPECT_VEC2_EQ(a * b, EX(T, 3, 10));

  if constexpr (std::is_floating_point_v<T>) {
    EXPECT_VEC2_EQ(b / a, EX(T, 3, 2.5));
  } else {
    EXPECT_VEC2_EQ(b / a, EX(T, 3, 2));
  }
}

TYPED_TEST(Vec2, EqualityAndInequality) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  const V a{T{1}, T{2}};
  const V b{T{1}, T{2}};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  const V c{T{1}, T{3}};
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
}

TYPED_TEST(Vec2, FloatingEqualityUsesEpsilon) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  if constexpr (!std::is_floating_point_v<T>) {
    GTEST_SKIP() << "epsilon equality applies only to floating-point types";
  } else {
    const V a{T{1}, T{2}};
    const V within{T{1} + (T{0.5} * mEn::kEpsilon<T>), T{2} - (T{0.5} * mEn::kEpsilon<T>)};
    EXPECT_TRUE(a == within);

    const V outside{T{1} + (T{2} * mEn::kEpsilon<T>), T{2}};
    EXPECT_FALSE(a == outside);
  }
}