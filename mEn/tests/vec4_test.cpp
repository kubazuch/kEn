#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mEn/constants.hpp>
#include <mEn/vec4.hpp>

#include "assert/vec4_eq.hpp"
#include "util/test_utils.hpp"

namespace {

template <typename T>
struct Vec4 : ::testing::Test {
  using V = mEn::vec<4, T>;
};

using TestedTypes = ::testing::Types<std::int32_t, float, double>;
TYPED_TEST_SUITE(Vec4, TestedTypes);

}  // namespace

TYPED_TEST(Vec4, LengthAndIndexing) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  V v{T{1}, T{2}, T{3}, T{4}};
  EXPECT_EQ(v.length(), 4U);

  EXPECT_EQ(v[0], v.x);
  EXPECT_EQ(v[1], v.y);
  EXPECT_EQ(v[2], v.z);
  EXPECT_EQ(v[3], v.w);

  v[0] = T{10};
  v[1] = T{20};
  v[2] = T{30};
  v[3] = T{40};
  EXPECT_VEC4_EQ(v, EX(T, 10, 20, 30, 40));

  const V cv{T{4}, T{5}, T{6}, T{7}};
  EXPECT_EQ(cv[0], cv.x);
  EXPECT_EQ(cv[1], cv.y);
  EXPECT_EQ(cv[2], cv.z);
  EXPECT_EQ(cv[3], cv.w);
}

TYPED_TEST(Vec4, AliasesShareStorage) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  V v{T{1}, T{2}, T{3}, T{4}};

  v.x = T{5};
  EXPECT_EQ(v.r, T{5});
  EXPECT_EQ(v.s, T{5});

  v.r = T{7};
  EXPECT_EQ(v.x, T{7});
  EXPECT_EQ(v.s, T{7});

  v.s = T{9};
  EXPECT_EQ(v.x, T{9});
  EXPECT_EQ(v.r, T{9});

  v.y = T{11};
  EXPECT_EQ(v.g, T{11});
  EXPECT_EQ(v.t, T{11});

  v.g = T{13};
  EXPECT_EQ(v.y, T{13});
  EXPECT_EQ(v.t, T{13});

  v.t = T{15};
  EXPECT_EQ(v.y, T{15});
  EXPECT_EQ(v.g, T{15});

  v.z = T{17};
  EXPECT_EQ(v.b, T{17});
  EXPECT_EQ(v.p, T{17});

  v.b = T{19};
  EXPECT_EQ(v.z, T{19});
  EXPECT_EQ(v.p, T{19});

  v.p = T{21};
  EXPECT_EQ(v.z, T{21});
  EXPECT_EQ(v.b, T{21});

  v.w = T{23};
  EXPECT_EQ(v.a, T{23});
  EXPECT_EQ(v.q, T{23});

  v.a = T{25};
  EXPECT_EQ(v.w, T{25});
  EXPECT_EQ(v.q, T{25});

  v.q = T{27};
  EXPECT_EQ(v.w, T{27});
  EXPECT_EQ(v.a, T{27});
}

TYPED_TEST(Vec4, Constructors) {
  using T = TypeParam;
  using V = typename TestFixture::V;
  using U = std::size_t;

  V splat{T{5}};
  EXPECT_VEC4_EQ(splat, EX(T, 5, 5, 5, 5));

  V quad{T{6}, T{7}, T{8}, T{9}};
  EXPECT_VEC4_EQ(quad, EX(T, 6, 7, 8, 9));

  V mixed{1, U{2}, 3, U{4}};
  EXPECT_VEC4_EQ(mixed, EX(T, 1, 2, 3, 4));

  mEn::vec<4, U> u{U{1}, U{2}, U{3}, U{4}};
  V from_u{u};
  EXPECT_VEC4_EQ(from_u, EX(T, 1, 2, 3, 4));
}

TYPED_TEST(Vec4, CompoundScalarArithmetic) {
  using T = TypeParam;
  using V = typename TestFixture::V;
  using U = std::size_t;

  V v{T{1}, T{2}, T{3}, T{4}};

  v += U{2};
  EXPECT_VEC4_EQ(v, EX(T, 3, 4, 5, 6));

  v -= U{1};
  EXPECT_VEC4_EQ(v, EX(T, 2, 3, 4, 5));

  v *= U{3};
  EXPECT_VEC4_EQ(v, EX(T, 6, 9, 12, 15));

  v /= U{3};
  EXPECT_VEC4_EQ(v, EX(T, 2, 3, 4, 5));
}

TYPED_TEST(Vec4, CompoundVectorArithmeticWithMixedTypes) {
  using T = TypeParam;
  using V = typename TestFixture::V;
  using U = std::size_t;

  V v{T{1}, T{2}, T{3}, T{4}};
  mEn::vec<4, U> i{2, 3, 4, 5};

  v += i;
  EXPECT_VEC4_EQ(v, EX(T, 3, 5, 7, 9));

  v -= i;
  EXPECT_VEC4_EQ(v, EX(T, 1, 2, 3, 4));

  v *= i;
  EXPECT_VEC4_EQ(v, EX(T, 2, 6, 12, 20));

  v /= i;
  EXPECT_VEC4_EQ(v, EX(T, 1, 2, 3, 4));

  v = i;
  EXPECT_VEC4_EQ(v, EX(T, 2, 3, 4, 5));
}

TYPED_TEST(Vec4, IncrementDecrement) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  V v{T{1}, T{2}, T{3}, T{4}};

  V post_inc = v++;
  EXPECT_VEC4_EQ(post_inc, EX(T, 1, 2, 3, 4));
  EXPECT_VEC4_EQ(v, EX(T, 2, 3, 4, 5));

  V pre_inc = ++v;
  EXPECT_VEC4_EQ(pre_inc, EX(T, 3, 4, 5, 6));
  EXPECT_VEC4_EQ(v, EX(T, 3, 4, 5, 6));

  V post_dec = v--;
  EXPECT_VEC4_EQ(post_dec, EX(T, 3, 4, 5, 6));
  EXPECT_VEC4_EQ(v, EX(T, 2, 3, 4, 5));

  V pre_dec = --v;
  EXPECT_VEC4_EQ(pre_dec, EX(T, 1, 2, 3, 4));
  EXPECT_VEC4_EQ(v, EX(T, 1, 2, 3, 4));
}

TYPED_TEST(Vec4, UnaryOperators) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  V v{T{2}, T{3}, T{4}, T{5}};

  V plus = +v;
  EXPECT_VEC4_EQ(plus, EX(T, 2, 3, 4, 5));

  V neg = -v;
  EXPECT_VEC4_EQ(neg, EX(T, -2, -3, -4, -5));
}

TYPED_TEST(Vec4, BinaryScalarOperators) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  const V v{T{1}, T{2}, T{3}, T{4}};

  EXPECT_VEC4_EQ(v + T{2}, EX(T, 3, 4, 5, 6));
  EXPECT_VEC4_EQ(T{2} + v, EX(T, 3, 4, 5, 6));

  EXPECT_VEC4_EQ(v - T{2}, EX(T, -1, 0, 1, 2));
  EXPECT_VEC4_EQ(T{5} - v, EX(T, 4, 3, 2, 1));

  EXPECT_VEC4_EQ(v * T{3}, EX(T, 3, 6, 9, 12));
  EXPECT_VEC4_EQ(T{3} * v, EX(T, 3, 6, 9, 12));

  if constexpr (std::is_floating_point_v<T>) {
    EXPECT_VEC4_EQ(v / T{2}, EX(T, 0.5, 1, 1.5, 2));
  } else {
    EXPECT_VEC4_EQ(v / T{2}, EX(T, 0, 1, 1, 2));
  }

  EXPECT_VEC4_EQ((T{12} / V{T{2}, T{3}, T{4}, T{6}}), EX(T, 6, 4, 3, 2));
}

TYPED_TEST(Vec4, BinaryVectorOperators) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  const V a{T{1}, T{2}, T{3}, T{4}};
  const V b{T{3}, T{5}, T{7}, T{9}};

  EXPECT_VEC4_EQ(a + b, EX(T, 4, 7, 10, 13));
  EXPECT_VEC4_EQ(a - b, EX(T, -2, -3, -4, -5));
  EXPECT_VEC4_EQ(a * b, EX(T, 3, 10, 21, 36));

  if constexpr (std::is_floating_point_v<T>) {
    EXPECT_VEC4_EQ(b / a, EX(T, 3, 2.5, (T{7} / T{3}), (T{9} / T{4})));
  } else {
    EXPECT_VEC4_EQ(b / a, EX(T, 3, 2, 2, 2));
  }
}

TYPED_TEST(Vec4, EqualityAndInequality) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  const V a{T{1}, T{2}, T{3}, T{4}};
  const V b{T{1}, T{2}, T{3}, T{4}};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  const V c{T{1}, T{3}, T{3}, T{4}};
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
}

TYPED_TEST(Vec4, FloatingEqualityUsesEpsilon) {
  using T = TypeParam;
  using V = typename TestFixture::V;

  if constexpr (!std::is_floating_point_v<T>) {
    GTEST_SKIP() << "epsilon equality applies only to floating-point types";
  } else {
    const V a{T{1}, T{2}, T{3}, T{4}};
    const auto half_eps = T{0.5} * mEn::kEpsilon<T>;

    const V within{
        T{1} + half_eps,
        T{2} - half_eps,
        T{3} + half_eps,
        T{4} - half_eps,
    };
    EXPECT_TRUE(a == within);

    const V outside{
        T{1} + (T{2} * mEn::kEpsilon<T>),
        T{2},
        T{3},
        T{4},
    };
    EXPECT_FALSE(a == outside);
  }
}