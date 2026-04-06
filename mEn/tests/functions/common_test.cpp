#include <gtest/gtest.h>

#include <limits>

#include <mEn/functions/common.hpp>
#include <mEn/fwd.hpp>

#include <glm/detail/qualifier.hpp>

#include "../assert/scalar_eq.hpp"
#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectBoolVecEq;
using men_test::ExpectVecEq;
using men_test::GV;
using men_test::MV;
using men_test::V;

template <mEn::length_t L, typename T>
void RunAbsSignTests() {
  const auto x = GV<L, T>(T{-1.5}, T{0}, T{2.25}, T{-3});
  ExpectVecEq(mEn::abs(MV(x)), glm::abs(x));
  ExpectVecEq(mEn::sign(MV(x)), glm::sign(x));

  const auto z = glm::vec<L, T>(T{0});
  ExpectVecEq(mEn::abs(MV(z)), glm::abs(z));
  ExpectVecEq(mEn::sign(MV(z)), glm::sign(z));
}

template <mEn::length_t L, typename T>
void RunRoundingTests() {
  const auto x    = GV<L, T>(T{-1.7}, T{-1.2}, T{1.2}, T{1.7});
  const auto ties = GV<L, T>(T{-2.5}, T{-1.5}, T{1.5}, T{2.5});
  ExpectVecEq(mEn::floor(MV(x)), glm::floor(x));
  ExpectVecEq(mEn::trunc(MV(x)), glm::trunc(x));
  ExpectVecEq(mEn::round(MV(x)), glm::round(x));
  ExpectVecEq(mEn::ceil(MV(x)), glm::ceil(x));
  ExpectVecEq(mEn::roundEven(MV(ties)), glm::roundEven(ties));
}

template <mEn::length_t L, typename T>
void RunFractModTests() {
  const auto x  = GV<L, T>(T{1.25}, T{-1.25}, T{3.0}, T{-0.1});
  const auto yv = GV<L, T>(T{2}, T{3}, T{4}, T{5});
  const T y     = T{2.0};
  ExpectVecEq(mEn::fract(MV(x)), glm::fract(x));
  ExpectVecEq(mEn::mod(MV(x), y), glm::mod(x, y));
  ExpectVecEq(mEn::mod(MV(x), MV(yv)), glm::mod(x, yv));
}

template <mEn::length_t L, typename T>
void RunMinMaxClampTests() {
  const auto x  = GV<L, T>(T{-1}, T{2}, T{5}, T{-4});
  const auto y  = GV<L, T>(T{0}, T{1}, T{6}, T{-10});
  const auto lo = GV<L, T>(T{-2}, T{0}, T{4}, T{-6});
  const auto hi = GV<L, T>(T{2}, T{3}, T{5}, T{-3});
  const T s     = T{1};
  const T from  = T{-2};
  const T to    = T{3};

  ExpectVecEq(mEn::min(MV(x), s), glm::min(x, s));
  ExpectVecEq(mEn::min(MV(x), MV(y)), glm::min(x, y));
  ExpectVecEq(mEn::max(MV(x), s), glm::max(x, s));
  ExpectVecEq(mEn::max(MV(x), MV(y)), glm::max(x, y));
  ExpectVecEq(mEn::clamp(MV(x), from, to), glm::clamp(x, from, to));
  ExpectVecEq(mEn::clamp(MV(x), MV(lo), MV(hi)), glm::clamp(x, lo, hi));
}

template <mEn::length_t L, typename T>
void RunMixTests() {
  const auto a  = GV<L, T>(T{0}, T{1}, T{2}, T{3});
  const auto b  = GV<L, T>(T{10}, T{11}, T{12}, T{13});
  const auto tv = GV<L, T>(T{0}, T{0.25}, T{0.5}, T{1});
  const T t     = T{0.25};

  ExpectVecEq(mEn::mix(MV(a), MV(b), t), glm::mix(a, b, t));
  ExpectVecEq(mEn::mix(MV(a), MV(b), MV(tv)), glm::mix(a, b, tv));
  ExpectVecEq(mEn::mix(MV(a), MV(b), T{0}), a);
  ExpectVecEq(mEn::mix(MV(a), MV(b), T{1}), b);
}

template <mEn::length_t L, typename T>
void RunStepSmoothstepTests() {
  const auto x   = GV<L, T>(T{0.0}, T{0.5}, T{0.49}, T{0.51});
  const auto ev  = GV<L, T>(T{0.0}, T{0.5}, T{0.5}, T{1.0});
  const auto xs  = GV<L, T>(T{-0.5}, T{0.0}, T{0.5}, T{1.5});
  const auto e1v = glm::vec<L, T>(T{0});
  const auto e2v = glm::vec<L, T>(T{1});
  const T edge   = T{0.5};
  const T e1     = T{0};
  const T e2     = T{1};

  ExpectVecEq(mEn::step(edge, MV(x)), glm::step(edge, x));
  ExpectVecEq(mEn::step(MV(ev), MV(x)), glm::step(ev, x));
  ExpectVecEq(mEn::smoothstep(e1, e2, MV(xs)), glm::smoothstep(e1, e2, xs));
  ExpectVecEq(mEn::smoothstep(MV(e1v), MV(e2v), MV(xs)), glm::smoothstep(e1v, e2v, xs));
}

template <mEn::length_t L, typename T>
void RunIsNanIsInfTests() {
  const T nan  = std::numeric_limits<T>::quiet_NaN();
  const T inf  = std::numeric_limits<T>::infinity();
  const auto x = GV<L, T>(nan, inf, -inf, T{1});
  ExpectBoolVecEq(mEn::isnan(MV(x)), glm::isnan(x));
  ExpectBoolVecEq(mEn::isinf(MV(x)), glm::isinf(x));
}

template <mEn::length_t L, typename T>
void RunWrapTests() {
  const auto x        = GV<L, T>(T{-0.5}, T{0.5}, T{1.0}, T{1.5});
  const auto lo       = GV<L, T>(T{0}, T{0}, T{0}, T{0});
  const auto hi       = GV<L, T>(T{1}, T{1}, T{1}, T{1});
  const auto expected = GV<L, T>(T{0.5}, T{0.5}, T{0}, T{0.5});
  ExpectVecEq(mEn::wrap(MV(x), MV(lo), MV(hi)), expected);

  ExpectVecEq(mEn::wrap(MV(x), T{0}, T{1}), expected);

  EXPECT_SCALAR_EQ(mEn::wrap(T{0.5}, T{0}, T{1}), T{0.5});
  EXPECT_SCALAR_EQ(mEn::wrap(T{0.0}, T{0}, T{1}), T{0.0});
  EXPECT_SCALAR_EQ(mEn::wrap(T{1.0}, T{0}, T{1}), T{0.0});
  EXPECT_SCALAR_EQ(mEn::wrap(T{1.5}, T{0}, T{1}), T{0.5});
  EXPECT_SCALAR_EQ(mEn::wrap(T{-0.5}, T{0}, T{1}), T{0.5});
  EXPECT_SCALAR_EQ(mEn::wrap(T{-0.5}, T{-1}, T{1}), T{-0.5});
  EXPECT_SCALAR_EQ(mEn::wrap(T{3.5}, T{-1}, T{1}), T{-0.5});
  EXPECT_SCALAR_EQ(mEn::wrap(T{-3.5}, T{-1}, T{1}), T{0.5});
}

template <typename T>
struct Vec2Common : ::testing::Test {};
template <typename T>
struct Vec3Common : ::testing::Test {};
template <typename T>
struct Vec4Common : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Vec2Common, TestedTypes);
TYPED_TEST_SUITE(Vec3Common, TestedTypes);
TYPED_TEST_SUITE(Vec4Common, TestedTypes);

}  // namespace

// ---- vec2 ----
TYPED_TEST(Vec2Common, AbsSign) { RunAbsSignTests<2, TypeParam>(); }
TYPED_TEST(Vec2Common, Rounding) { RunRoundingTests<2, TypeParam>(); }
TYPED_TEST(Vec2Common, FractMod) { RunFractModTests<2, TypeParam>(); }
TYPED_TEST(Vec2Common, MinMaxClamp) { RunMinMaxClampTests<2, TypeParam>(); }
TYPED_TEST(Vec2Common, Mix) { RunMixTests<2, TypeParam>(); }
TYPED_TEST(Vec2Common, StepSmoothstep) { RunStepSmoothstepTests<2, TypeParam>(); }
TYPED_TEST(Vec2Common, IsNanIsInf) { RunIsNanIsInfTests<2, TypeParam>(); }
TYPED_TEST(Vec2Common, Wrap) { RunWrapTests<2, TypeParam>(); }

// ---- vec3 ----
TYPED_TEST(Vec3Common, AbsSign) { RunAbsSignTests<3, TypeParam>(); }
TYPED_TEST(Vec3Common, Rounding) { RunRoundingTests<3, TypeParam>(); }
TYPED_TEST(Vec3Common, FractMod) { RunFractModTests<3, TypeParam>(); }
TYPED_TEST(Vec3Common, MinMaxClamp) { RunMinMaxClampTests<3, TypeParam>(); }
TYPED_TEST(Vec3Common, Mix) { RunMixTests<3, TypeParam>(); }
TYPED_TEST(Vec3Common, StepSmoothstep) { RunStepSmoothstepTests<3, TypeParam>(); }
TYPED_TEST(Vec3Common, IsNanIsInf) { RunIsNanIsInfTests<3, TypeParam>(); }
TYPED_TEST(Vec3Common, Wrap) { RunWrapTests<3, TypeParam>(); }

// ---- vec4 ----
TYPED_TEST(Vec4Common, AbsSign) { RunAbsSignTests<4, TypeParam>(); }
TYPED_TEST(Vec4Common, Rounding) { RunRoundingTests<4, TypeParam>(); }
TYPED_TEST(Vec4Common, FractMod) { RunFractModTests<4, TypeParam>(); }
TYPED_TEST(Vec4Common, MinMaxClamp) { RunMinMaxClampTests<4, TypeParam>(); }
TYPED_TEST(Vec4Common, Mix) { RunMixTests<4, TypeParam>(); }
TYPED_TEST(Vec4Common, StepSmoothstep) { RunStepSmoothstepTests<4, TypeParam>(); }
TYPED_TEST(Vec4Common, IsNanIsInf) { RunIsNanIsInfTests<4, TypeParam>(); }
TYPED_TEST(Vec4Common, Wrap) { RunWrapTests<4, TypeParam>(); }
