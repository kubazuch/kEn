#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mEn/constants.hpp>
#include <mEn/fwd.hpp>
#include <mEn/vec3.hpp>

#include "assert/vec3_eq.hpp"
#include "util/test_utils.hpp"

namespace {

template <typename T>
struct Vec3 : ::testing::Test {
  using V = mEn::vec<3, T>;
};

using TestedTypes = ::testing::Types<std::int32_t, float, double>;
TYPED_TEST_SUITE(Vec3, TestedTypes);

}  // namespace

TYPED_TEST(Vec3, LengthAndIndexing) {
  // NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
  using T = TypeParam;
  using V = TestFixture::V;

  V v{T{1}, T{2}, T{3}};
  EXPECT_EQ(v.length(), 3U);

  EXPECT_EQ(v[0], v.x);
  EXPECT_EQ(v[1], v.y);
  EXPECT_EQ(v[2], v.z);

  v[0] = T{10};
  v[1] = T{20};
  v[2] = T{30};
  EXPECT_VEC3_EQ(v, EX(T, 10, 20, 30));

  const V cv{T{4}, T{5}, T{6}};
  EXPECT_EQ(cv[0], cv.x);
  EXPECT_EQ(cv[1], cv.y);
  EXPECT_EQ(cv[2], cv.z);
  // NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}

TYPED_TEST(Vec3, AliasesShareStorage) {
  using T = TypeParam;
  using V = TestFixture::V;

  V v{T{1}, T{2}, T{3}};

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
}

TYPED_TEST(Vec3, Constructors) {
  using T = TypeParam;
  using V = TestFixture::V;
  using U = std::size_t;

  const V splat{T{5}};
  EXPECT_VEC3_EQ(splat, EX(T, 5, 5, 5));

  const V triple{T{6}, T{7}, T{8}};
  EXPECT_VEC3_EQ(triple, EX(T, 6, 7, 8));

  const V mixed{1, U{2}, 3};
  EXPECT_VEC3_EQ(mixed, EX(T, 1, 2, 3));

  const mEn::vec<3, U> u{U{1}, U{2}, U{3}};
  const V from_u{u};
  EXPECT_VEC3_EQ(from_u, EX(T, 1, 2, 3));
}

TYPED_TEST(Vec3, CompoundScalarArithmetic) {
  using T = TypeParam;
  using V = TestFixture::V;
  using U = std::size_t;

  V v{T{1}, T{2}, T{3}};

  v += U{2};
  EXPECT_VEC3_EQ(v, EX(T, 3, 4, 5));

  v -= U{1};
  EXPECT_VEC3_EQ(v, EX(T, 2, 3, 4));

  v *= U{3};
  EXPECT_VEC3_EQ(v, EX(T, 6, 9, 12));

  v /= U{3};
  EXPECT_VEC3_EQ(v, EX(T, 2, 3, 4));
}

TYPED_TEST(Vec3, CompoundVectorArithmeticWithMixedTypes) {
  using T = TypeParam;
  using V = TestFixture::V;
  using U = std::size_t;

  V v{T{1}, T{2}, T{3}};
  const mEn::vec<3, U> i{2, 3, 4};

  v += i;
  EXPECT_VEC3_EQ(v, EX(T, 3, 5, 7));

  v -= i;
  EXPECT_VEC3_EQ(v, EX(T, 1, 2, 3));

  v *= i;
  EXPECT_VEC3_EQ(v, EX(T, 2, 6, 12));

  v /= i;
  EXPECT_VEC3_EQ(v, EX(T, 1, 2, 3));

  v = i;
  EXPECT_VEC3_EQ(v, EX(T, 2, 3, 4));
}

TYPED_TEST(Vec3, IncrementDecrement) {
  using T = TypeParam;
  using V = TestFixture::V;

  V v{T{1}, T{2}, T{3}};

  const V post_inc = v++;
  EXPECT_VEC3_EQ(post_inc, EX(T, 1, 2, 3));
  EXPECT_VEC3_EQ(v, EX(T, 2, 3, 4));

  const V pre_inc = ++v;
  EXPECT_VEC3_EQ(pre_inc, EX(T, 3, 4, 5));
  EXPECT_VEC3_EQ(v, EX(T, 3, 4, 5));

  const V post_dec = v--;
  EXPECT_VEC3_EQ(post_dec, EX(T, 3, 4, 5));
  EXPECT_VEC3_EQ(v, EX(T, 2, 3, 4));

  const V pre_dec = --v;
  EXPECT_VEC3_EQ(pre_dec, EX(T, 1, 2, 3));
  EXPECT_VEC3_EQ(v, EX(T, 1, 2, 3));
}

TYPED_TEST(Vec3, UnaryOperators) {
  using T = TypeParam;
  using V = TestFixture::V;

  const V v{T{2}, T{3}, T{4}};

  const V plus = +v;
  EXPECT_VEC3_EQ(plus, EX(T, 2, 3, 4));

  const V neg = -v;
  EXPECT_VEC3_EQ(neg, EX(T, -2, -3, -4));
}

TYPED_TEST(Vec3, BinaryScalarOperators) {
  using T = TypeParam;
  using V = TestFixture::V;

  const V v{T{1}, T{2}, T{3}};

  EXPECT_VEC3_EQ(v + T{2}, EX(T, 3, 4, 5));
  EXPECT_VEC3_EQ(T{2} + v, EX(T, 3, 4, 5));

  EXPECT_VEC3_EQ(v - T{2}, EX(T, -1, 0, 1));
  EXPECT_VEC3_EQ(T{5} - v, EX(T, 4, 3, 2));

  EXPECT_VEC3_EQ(v * T{3}, EX(T, 3, 6, 9));
  EXPECT_VEC3_EQ(T{3} * v, EX(T, 3, 6, 9));

  if constexpr (std::is_floating_point_v<T>) {
    EXPECT_VEC3_EQ(v / T{2}, EX(T, 0.5, 1, 1.5));
  } else {
    EXPECT_VEC3_EQ(v / T{2}, EX(T, 0, 1, 1));
  }

  EXPECT_VEC3_EQ((T{6} / V{T{2}, T{3}, T{6}}), EX(T, 3, 2, 1));
}

TYPED_TEST(Vec3, BinaryVectorOperators) {
  using T = TypeParam;
  using V = TestFixture::V;

  const V a{T{1}, T{2}, T{3}};
  const V b{T{3}, T{5}, T{7}};

  EXPECT_VEC3_EQ(a + b, EX(T, 4, 7, 10));
  EXPECT_VEC3_EQ(a - b, EX(T, -2, -3, -4));
  EXPECT_VEC3_EQ(a * b, EX(T, 3, 10, 21));

  if constexpr (std::is_floating_point_v<T>) {
    EXPECT_VEC3_EQ(b / a, EX(T, 3, 2.5, (T{7} / T{3})));
  } else {
    EXPECT_VEC3_EQ(b / a, EX(T, 3, 2, 2));
  }
}

TYPED_TEST(Vec3, EqualityAndInequality) {
  using T = TypeParam;
  using V = TestFixture::V;

  const V a{T{1}, T{2}, T{3}};
  const V b{T{1}, T{2}, T{3}};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  const V c{T{1}, T{3}, T{3}};
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
}

TYPED_TEST(Vec3, FloatingEqualityUsesEpsilon) {
  using T = TypeParam;
  using V = TestFixture::V;

  if constexpr (!std::is_floating_point_v<T>) {
    GTEST_SKIP() << "epsilon equality applies only to floating-point types";
  } else {
    const V a{T{1}, T{2}, T{3}};
    const auto half_eps = T{0.5} * mEn::kEpsilon<T>;

    const V within{
        T{1} + half_eps,
        T{2} - half_eps,
        T{3} + half_eps,
    };
    EXPECT_TRUE(a == within);

    const V outside{
        T{1} + (T{2} * mEn::kEpsilon<T>),
        T{2},
        T{3},
    };
    EXPECT_FALSE(a == outside);
  }
}