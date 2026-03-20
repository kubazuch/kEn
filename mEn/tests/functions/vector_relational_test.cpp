#include <gtest/gtest.h>

#include <mEn/constants.hpp>
#include <mEn/functions/vector_relational.hpp>
#include <mEn/fwd.hpp>

#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_relational.hpp>

#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectBoolVecEq;
using men_test::GV;
using men_test::MV;
using men_test::V;

template <mEn::length_t L, typename T>
void RunComparisonTests() {
  const auto a = GV<L, T>(T{1}, T{2}, T{3}, T{4});
  const auto b = GV<L, T>(T{1}, T{3}, T{2}, T{4});

  ExpectBoolVecEq(mEn::lessThan(MV(a), MV(b)), glm::lessThan(a, b));
  ExpectBoolVecEq(mEn::lessThanEqual(MV(a), MV(b)), glm::lessThanEqual(a, b));
  ExpectBoolVecEq(mEn::greaterThan(MV(a), MV(b)), glm::greaterThan(a, b));
  ExpectBoolVecEq(mEn::greaterThanEqual(MV(a), MV(b)), glm::greaterThanEqual(a, b));
  ExpectBoolVecEq(mEn::equal(MV(a), MV(a)), glm::equal(a, a));
  ExpectBoolVecEq(mEn::notEqual(MV(a), MV(a)), glm::notEqual(a, a));
}

template <mEn::length_t L, typename T>
void RunEqualEpsilonTests() {
  auto base   = GV<L, T>(T{1}, T{2}, T{3}, T{4});
  auto approx = base;

  const T eps = mEn::kEpsilon<T>;
  approx[0]   = base[0] + (eps * T{0.5});
  approx[1]   = base[1] + (eps * T{2});

  const auto eq = mEn::equal(MV(base), MV(approx));

  ExpectBoolVecEq(eq, glm::equal(base, approx, eps));
  ExpectBoolVecEq(mEn::notEqual(MV(base), MV(approx)), glm::notEqual(base, approx, eps));

  EXPECT_TRUE(mEn::any(eq));
  EXPECT_FALSE(mEn::all(eq));
}

template <mEn::length_t L, typename T>
void RunAnyAllNotTests() {
  const auto all_false = glm::vec<L, bool>(false);
  const auto all_true  = glm::vec<L, bool>(true);
  const auto mixed     = GV<L, bool>(true, false, true, false);

  EXPECT_FALSE(mEn::any(MV(all_false)));
  EXPECT_TRUE(mEn::all(MV(all_true)));
  EXPECT_TRUE(mEn::any(MV(mixed)));
  EXPECT_FALSE(mEn::all(MV(mixed)));

  ExpectBoolVecEq(mEn::not_(MV(mixed)), glm::not_(mixed));
}

template <mEn::length_t L, typename T>
void RunScalarComparisonTests() {
  const auto a = MV(GV<L, T>(T{1}, T{2}, T{3}, T{4}));
  const T s    = T{2};

  ExpectBoolVecEq(mEn::lessThan(a, s), mEn::lessThan(a, mEn::vec<L, T>(s)));
  ExpectBoolVecEq(mEn::lessThanEqual(a, s), mEn::lessThanEqual(a, mEn::vec<L, T>(s)));
  ExpectBoolVecEq(mEn::greaterThan(a, s), mEn::greaterThan(a, mEn::vec<L, T>(s)));
  ExpectBoolVecEq(mEn::greaterThanEqual(a, s), mEn::greaterThanEqual(a, mEn::vec<L, T>(s)));
  ExpectBoolVecEq(mEn::equal(a, s), mEn::equal(a, mEn::vec<L, T>(s)));
  ExpectBoolVecEq(mEn::notEqual(a, s), mEn::notEqual(a, mEn::vec<L, T>(s)));
}

template <typename T>
struct Vec2Relational : ::testing::Test {};
template <typename T>
struct Vec3Relational : ::testing::Test {};
template <typename T>
struct Vec4Relational : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Vec2Relational, TestedTypes);
TYPED_TEST_SUITE(Vec3Relational, TestedTypes);
TYPED_TEST_SUITE(Vec4Relational, TestedTypes);

}  // namespace

TYPED_TEST(Vec2Relational, Comparisons) { RunComparisonTests<2, TypeParam>(); }
TYPED_TEST(Vec2Relational, ScalarComparisons) { RunScalarComparisonTests<2, TypeParam>(); }
TYPED_TEST(Vec2Relational, EqualUsesEpsilonForFloating) { RunEqualEpsilonTests<2, TypeParam>(); }
TYPED_TEST(Vec2Relational, AnyAllNot) { RunAnyAllNotTests<2, TypeParam>(); }

TYPED_TEST(Vec3Relational, Comparisons) { RunComparisonTests<3, TypeParam>(); }
TYPED_TEST(Vec3Relational, ScalarComparisons) { RunScalarComparisonTests<3, TypeParam>(); }
TYPED_TEST(Vec3Relational, EqualUsesEpsilonForFloating) { RunEqualEpsilonTests<3, TypeParam>(); }
TYPED_TEST(Vec3Relational, AnyAllNot) { RunAnyAllNotTests<3, TypeParam>(); }

TYPED_TEST(Vec4Relational, Comparisons) { RunComparisonTests<4, TypeParam>(); }
TYPED_TEST(Vec4Relational, ScalarComparisons) { RunScalarComparisonTests<4, TypeParam>(); }
TYPED_TEST(Vec4Relational, EqualUsesEpsilonForFloating) { RunEqualEpsilonTests<4, TypeParam>(); }
TYPED_TEST(Vec4Relational, AnyAllNot) { RunAnyAllNotTests<4, TypeParam>(); }
