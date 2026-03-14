#include <gtest/gtest.h>

#include <mEn/functions/exponential.hpp>

#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectVecEq;
using men_test::GV;
using men_test::MV;
using men_test::V;

template <mEn::length_t L, typename T>
void RunPowTests() {
  const auto base     = GV<L, T>(T{1}, T{2}, T{3}, T{4});
  const auto exponent = GV<L, T>(T{0}, T{0.5}, T{2}, T{3});
  const auto zero     = glm::vec<L, T>(T{0});
  const auto one      = glm::vec<L, T>(T{1});

  ExpectVecEq(mEn::pow(MV(base), MV(exponent)), glm::pow(base, exponent));
  ExpectVecEq(mEn::pow(MV(base), MV(zero)), glm::pow(base, zero));
  ExpectVecEq(mEn::pow(MV(base), MV(one)), glm::pow(base, one));
}

template <mEn::length_t L, typename T>
void RunExpLogTests() {
  const auto x    = GV<L, T>(T{0}, T{0.5}, T{-1}, T{2});
  const auto pos  = GV<L, T>(T{1}, T{0.5}, T{2}, T{10});
  const auto zero = glm::vec<L, T>(T{0});
  const auto ones = glm::vec<L, T>(T{1});

  ExpectVecEq(mEn::exp(MV(x)), glm::exp(x));
  ExpectVecEq(mEn::log(MV(pos)), glm::log(pos));
  ExpectVecEq(mEn::exp(MV(zero)), glm::exp(zero));
  ExpectVecEq(mEn::log(MV(ones)), glm::log(ones));
}

template <mEn::length_t L, typename T>
void RunExp2Log2Tests() {
  const auto x    = GV<L, T>(T{0}, T{1}, T{-2}, T{3});
  const auto pos  = GV<L, T>(T{1}, T{0.5}, T{2}, T{8});
  const auto zero = glm::vec<L, T>(T{0});
  const auto ones = glm::vec<L, T>(T{1});

  ExpectVecEq(mEn::exp2(MV(x)), glm::exp2(x));
  ExpectVecEq(mEn::log2(MV(pos)), glm::log2(pos));
  ExpectVecEq(mEn::exp2(MV(zero)), glm::exp2(zero));
  ExpectVecEq(mEn::log2(MV(ones)), glm::log2(ones));
}

template <mEn::length_t L, typename T>
void RunExp10Log10Tests() {
  const auto x   = GV<L, T>(T{1}, T{10}, T{100}, T{0.1});
  const auto ref = GV<L, T>(T{0}, T{1}, T{2}, T{-1});

  ExpectVecEq(mEn::log10(MV(x)), ref);
}

template <mEn::length_t L, typename T>
void RunSqrtInverseSqrtTests() {
  const auto x    = GV<L, T>(T{0}, T{0.25}, T{4}, T{9});
  const auto pos  = GV<L, T>(T{1}, T{4}, T{9}, T{16});
  const auto zero = glm::vec<L, T>(T{0});
  const auto ones = glm::vec<L, T>(T{1});

  ExpectVecEq(mEn::sqrt(MV(x)), glm::sqrt(x));
  ExpectVecEq(mEn::inversesqrt(MV(pos)), glm::inversesqrt(pos));
  ExpectVecEq(mEn::sqrt(MV(zero)), glm::sqrt(zero));
  ExpectVecEq(mEn::sqrt(MV(ones)), glm::sqrt(ones));
  ExpectVecEq(mEn::inversesqrt(MV(ones)), glm::inversesqrt(ones));
}

template <typename T>
struct Vec2Exp : ::testing::Test {};
template <typename T>
struct Vec3Exp : ::testing::Test {};
template <typename T>
struct Vec4Exp : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Vec2Exp, TestedTypes);
TYPED_TEST_SUITE(Vec3Exp, TestedTypes);
TYPED_TEST_SUITE(Vec4Exp, TestedTypes);

}  // namespace

// ---- vec2 ----
TYPED_TEST(Vec2Exp, Pow) { RunPowTests<2, TypeParam>(); }
TYPED_TEST(Vec2Exp, ExpLog) { RunExpLogTests<2, TypeParam>(); }
TYPED_TEST(Vec2Exp, Exp2Log2) { RunExp2Log2Tests<2, TypeParam>(); }
TYPED_TEST(Vec2Exp, Exp10Log10) { RunExp10Log10Tests<2, TypeParam>(); }
TYPED_TEST(Vec2Exp, SqrtInverseSqrt) { RunSqrtInverseSqrtTests<2, TypeParam>(); }

// ---- vec3 ----
TYPED_TEST(Vec3Exp, Pow) { RunPowTests<3, TypeParam>(); }
TYPED_TEST(Vec3Exp, ExpLog) { RunExpLogTests<3, TypeParam>(); }
TYPED_TEST(Vec3Exp, Exp2Log2) { RunExp2Log2Tests<3, TypeParam>(); }
TYPED_TEST(Vec3Exp, Exp10Log10) { RunExp10Log10Tests<3, TypeParam>(); }
TYPED_TEST(Vec3Exp, SqrtInverseSqrt) { RunSqrtInverseSqrtTests<3, TypeParam>(); }

// ---- vec4 ----
TYPED_TEST(Vec4Exp, Pow) { RunPowTests<4, TypeParam>(); }
TYPED_TEST(Vec4Exp, ExpLog) { RunExpLogTests<4, TypeParam>(); }
TYPED_TEST(Vec4Exp, Exp2Log2) { RunExp2Log2Tests<4, TypeParam>(); }
TYPED_TEST(Vec4Exp, Exp10Log10) { RunExp10Log10Tests<4, TypeParam>(); }
TYPED_TEST(Vec4Exp, SqrtInverseSqrt) { RunSqrtInverseSqrtTests<4, TypeParam>(); }