#include <gtest/gtest.h>

#include <mEn/functions/trigonometric.hpp>
#include <mEn/fwd.hpp>

#include <glm/detail/qualifier.hpp>

#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectVecEq;
using men_test::GV;
using men_test::MV;
using men_test::V;

template <mEn::length_t L, typename T>
void RunRadiansDegreesTests() {
  const auto deg = GV<L, T>(T{0}, T{45}, T{-90}, T{180});
  const auto rad = glm::radians(deg);

  ExpectVecEq(mEn::radians(MV(deg)), rad);
  ExpectVecEq(mEn::degrees(MV(rad)), glm::degrees(rad));
}

template <mEn::length_t L, typename T>
void RunTrigTests() {
  const auto angle = GV<L, T>(T{0}, T{0.5}, T{-1}, T{1.5});
  const auto zero  = glm::vec<L, T>(T{0});

  ExpectVecEq(mEn::sin(MV(angle)), glm::sin(angle));
  ExpectVecEq(mEn::cos(MV(angle)), glm::cos(angle));
  ExpectVecEq(mEn::tan(MV(angle)), glm::tan(angle));

  ExpectVecEq(mEn::sin(MV(zero)), glm::sin(zero));
  ExpectVecEq(mEn::cos(MV(zero)), glm::cos(zero));
  ExpectVecEq(mEn::tan(MV(zero)), glm::tan(zero));
}

template <mEn::length_t L, typename T>
void RunInverseTrigTests() {
  const auto x   = GV<L, T>(T{0}, T{0.5}, T{-0.5}, T{1});
  const auto y   = GV<L, T>(T{0}, T{1}, T{-1}, T{0.5});
  const auto x2  = GV<L, T>(T{1}, T{-1}, T{2}, T{-2});
  const auto any = GV<L, T>(T{0}, T{1}, T{-2}, T{3.5});

  ExpectVecEq(mEn::asin(MV(x)), glm::asin(x));
  ExpectVecEq(mEn::acos(MV(x)), glm::acos(x));
  ExpectVecEq(mEn::atan(MV(any)), glm::atan(any));
  ExpectVecEq(mEn::atan(MV(y), MV(x2)), glm::atan(y, x2));
}

template <mEn::length_t L, typename T>
void RunHyperbolicTests() {
  const auto angle = GV<L, T>(T{0}, T{0.5}, T{-1}, T{2});

  ExpectVecEq(mEn::sinh(MV(angle)), glm::sinh(angle));
  ExpectVecEq(mEn::cosh(MV(angle)), glm::cosh(angle));
  ExpectVecEq(mEn::tanh(MV(angle)), glm::tanh(angle));
}

template <mEn::length_t L, typename T>
void RunInverseHyperbolicTests() {
  const auto any  = GV<L, T>(T{0}, T{0.5}, T{-2}, T{3});
  const auto ge1  = GV<L, T>(T{1}, T{1.5}, T{2}, T{10});
  const auto in01 = GV<L, T>(T{0}, T{0.2}, T{-0.5}, T{0.9});

  ExpectVecEq(mEn::asinh(MV(any)), glm::asinh(any));
  ExpectVecEq(mEn::acosh(MV(ge1)), glm::acosh(ge1));
  ExpectVecEq(mEn::atanh(MV(in01)), glm::atanh(in01));
}

template <typename T>
struct Vec2Trig : ::testing::Test {};
template <typename T>
struct Vec3Trig : ::testing::Test {};
template <typename T>
struct Vec4Trig : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Vec2Trig, TestedTypes);
TYPED_TEST_SUITE(Vec3Trig, TestedTypes);
TYPED_TEST_SUITE(Vec4Trig, TestedTypes);

}  // namespace

// ---- vec2 ----
TYPED_TEST(Vec2Trig, RadiansDegrees) { RunRadiansDegreesTests<2, TypeParam>(); }
TYPED_TEST(Vec2Trig, Trig) { RunTrigTests<2, TypeParam>(); }
TYPED_TEST(Vec2Trig, InverseTrig) { RunInverseTrigTests<2, TypeParam>(); }
TYPED_TEST(Vec2Trig, Hyperbolic) { RunHyperbolicTests<2, TypeParam>(); }
TYPED_TEST(Vec2Trig, InverseHyperbolic) { RunInverseHyperbolicTests<2, TypeParam>(); }

// ---- vec3 ----
TYPED_TEST(Vec3Trig, RadiansDegrees) { RunRadiansDegreesTests<3, TypeParam>(); }
TYPED_TEST(Vec3Trig, Trig) { RunTrigTests<3, TypeParam>(); }
TYPED_TEST(Vec3Trig, InverseTrig) { RunInverseTrigTests<3, TypeParam>(); }
TYPED_TEST(Vec3Trig, Hyperbolic) { RunHyperbolicTests<3, TypeParam>(); }
TYPED_TEST(Vec3Trig, InverseHyperbolic) { RunInverseHyperbolicTests<3, TypeParam>(); }

// ---- vec4 ----
TYPED_TEST(Vec4Trig, RadiansDegrees) { RunRadiansDegreesTests<4, TypeParam>(); }
TYPED_TEST(Vec4Trig, Trig) { RunTrigTests<4, TypeParam>(); }
TYPED_TEST(Vec4Trig, InverseTrig) { RunInverseTrigTests<4, TypeParam>(); }
TYPED_TEST(Vec4Trig, Hyperbolic) { RunHyperbolicTests<4, TypeParam>(); }
TYPED_TEST(Vec4Trig, InverseHyperbolic) { RunInverseHyperbolicTests<4, TypeParam>(); }
