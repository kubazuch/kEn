#include <gtest/gtest.h>

#include <limits>
#include <type_traits>

#include <mEn/config.hpp>
#include <mEn/detail/vectorize.hpp>
#include <mEn/fwd.hpp>
#include <mEn/vec3.hpp>

#include "../assert/vec2_eq.hpp"
#include "../assert/vec3_eq.hpp"
#include "../assert/vec4_eq.hpp"
#include "../util/test_utils.hpp"

namespace {

TEST(Vectorize, MapNegation) {
  constexpr auto kNeg = [](float x) -> float { return -x; };
  const mEn::Vec3 v{1.0F, -2.0F, 3.0F};
  const auto result = mEn::detail::map<kNeg>(v);

  EXPECT_VEC3_EQ(result, EX(float, -1.0F, 2.0F, -3.0F));
}

TEST(Vectorize, MapSquare) {
  constexpr auto kSq = [](float x) -> float { return x * x; };
  const mEn::Vec2 v{3.0F, 4.0F};
  const auto result = mEn::detail::map<kSq>(v);

  EXPECT_VEC2_EQ(result, EX(float, 9.0F, 16.0F));
}

TEST(Vectorize, MapTypeConversion) {
  constexpr auto kToFloat = [](int x) -> float { return static_cast<float>(x) * 0.5F; };
  const mEn::vec<3, int> v{2, 4, 6};
  const auto result = mEn::detail::map<kToFloat>(v);

  static_assert(std::is_same_v<std::remove_const_t<decltype(result)>, mEn::vec<3, float>>);
  EXPECT_VEC3_EQ(result, EX(float, 1.0F, 2.0F, 3.0F));
}

TEST(Vectorize, MapVec4) {
  constexpr auto kAbsFn = [](float x) -> float { return x < 0.0F ? -x : x; };
  const mEn::Vec4 v{-1.0F, 2.0F, -3.0F, 4.0F};
  const auto result = mEn::detail::map<kAbsFn>(v);

  EXPECT_VEC4_EQ(result, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(Vectorize, ZipAddTwoVectors) {
  constexpr auto kAdd = [](float a, float b) -> float { return a + b; };
  const mEn::Vec3 a{1.0F, 2.0F, 3.0F};
  const mEn::Vec3 b{4.0F, 5.0F, 6.0F};
  const auto result = mEn::detail::zip<kAdd>(a, b);

  EXPECT_VEC3_EQ(result, EX(float, 5.0F, 7.0F, 9.0F));
}

TEST(Vectorize, ZipVecAndScalar) {
  constexpr auto kMul = [](float v, float s) -> float { return v * s; };
  const mEn::Vec3 v{1.0F, 2.0F, 3.0F};
  const float scalar = 2.0F;
  const auto result  = mEn::detail::zip<kMul>(v, scalar);

  EXPECT_VEC3_EQ(result, EX(float, 2.0F, 4.0F, 6.0F));
}

TEST(Vectorize, ZipThreeArgs) {
  constexpr auto kFmaFn = [](float a, float b, float c) -> float { return (a * b) + c; };
  const mEn::Vec3 a{1.0F, 2.0F, 3.0F};
  const mEn::Vec3 b{2.0F, 3.0F, 4.0F};
  const mEn::Vec3 c{0.5F, 0.5F, 0.5F};
  const auto result = mEn::detail::zip<kFmaFn>(a, b, c);

  EXPECT_VEC3_EQ(result, EX(float, 2.5F, 6.5F, 12.5F));
}

TEST(Vectorize, ZipSingleVec) {
  constexpr auto kDbl = [](float x) -> float { return x * 2.0F; };
  const mEn::Vec2 v{3.0F, 7.0F};
  const auto result = mEn::detail::zip<kDbl>(v);

  EXPECT_VEC2_EQ(result, EX(float, 6.0F, 14.0F));
}

TEST(Vectorize, ReduceSum) {
  constexpr auto kAdd = [](float acc, float x) -> float { return acc + x; };
  const mEn::Vec3 v{1.0F, 2.0F, 3.0F};
  const float sum = mEn::detail::reduce<kAdd>(v, 0.0F);

  EXPECT_NEAR(sum, 6.0F, mEn::kEpsilon<>);
}

TEST(Vectorize, ReduceProduct) {
  constexpr auto kMul = [](float acc, float x) -> float { return acc * x; };
  const mEn::Vec3 v{2.0F, 3.0F, 4.0F};
  const float result = mEn::detail::reduce<kMul>(v, 1.0F);

  EXPECT_NEAR(result, 24.0F, mEn::kEpsilon<>);
}

TEST(Vectorize, ReduceMax) {
  constexpr auto kMaxFn = [](float acc, float x) -> float { return acc > x ? acc : x; };
  const mEn::Vec4 v{3.0F, 1.0F, 4.0F, 2.0F};
  const float result = mEn::detail::reduce<kMaxFn>(v, std::numeric_limits<float>::lowest());

  EXPECT_NEAR(result, 4.0F, mEn::kEpsilon<>);
}

TEST(Vectorize, ReduceIsLeftFold) {
  constexpr auto kSub = [](float acc, float x) -> float { return acc - x; };
  const mEn::Vec3 v{1.0F, 2.0F, 3.0F};
  const float result = mEn::detail::reduce<kSub>(v, 10.0F);

  EXPECT_NEAR(result, 4.0F, mEn::kEpsilon<>);
}

}  // namespace
