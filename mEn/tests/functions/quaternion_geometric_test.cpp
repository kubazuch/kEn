#include <gtest/gtest.h>

#include <mEn/functions/quaternion_geometric.hpp>

#include <glm/ext/quaternion_geometric.hpp>

#include "../assert/scalar_eq.hpp"
#include "../util/quat_test_utils.hpp"

namespace {

using men_test::ExpectQuatEq;
using men_test::GQ;
using men_test::MQ;
using men_test::Q;

template <typename T>
struct QuatGeometric : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(QuatGeometric, TestedTypes);

}  // namespace

TYPED_TEST(QuatGeometric, LengthOfIdentityIsOne) {
  using T = TypeParam;

  const auto gq = GQ(T{1}, T{0}, T{0}, T{0});
  EXPECT_SCALAR_EQ(mEn::length(MQ(gq)), glm::length(gq));
}

TYPED_TEST(QuatGeometric, LengthGeneral) {
  using T = TypeParam;

  const auto gq = GQ(T{2}, T{1}, T{2}, T{2});
  EXPECT_SCALAR_EQ(mEn::length(MQ(gq)), glm::length(gq));
}

TYPED_TEST(QuatGeometric, LengthMatchesSqrtOfDotWithSelf) {
  using T = TypeParam;

  const auto gq = GQ(T{1}, T{3}, T{-2}, T{4});
  EXPECT_SCALAR_EQ(mEn::length(MQ(gq)), glm::length(gq));
}

TYPED_TEST(QuatGeometric, NormalizeProducesUnitLength) {
  using T = TypeParam;

  const auto gq = GQ(T{1}, T{3}, T{-2}, T{4});
  EXPECT_SCALAR_EQ(mEn::length(mEn::normalize(MQ(gq))), T{1});
}

TYPED_TEST(QuatGeometric, NormalizeIdentityIsIdentity) {
  using T = TypeParam;

  const auto gq = GQ(T{1}, T{0}, T{0}, T{0});
  ExpectQuatEq(mEn::normalize(MQ(gq)), glm::normalize(gq));
}

TYPED_TEST(QuatGeometric, DotProduct) {
  using T = TypeParam;

  const auto ga = GQ(T{1}, T{2}, T{3}, T{4});
  const auto gb = GQ(T{5}, T{6}, T{7}, T{8});
  EXPECT_SCALAR_EQ(mEn::dot(MQ(ga), MQ(gb)), glm::dot(ga, gb));
  EXPECT_SCALAR_EQ(mEn::dot(MQ(gb), MQ(ga)), glm::dot(gb, ga));
}

TYPED_TEST(QuatGeometric, DotOfNormalizedWithSelfIsOne) {
  using T = TypeParam;

  const auto gq = GQ(T{2}, T{1}, T{-3}, T{5});
  const auto gn = glm::normalize(gq);
  EXPECT_SCALAR_EQ(mEn::dot(MQ(gn), MQ(gn)), T{1});
}

TYPED_TEST(QuatGeometric, CrossEqualsHamiltonProduct) {
  using T = TypeParam;

  const auto gqi = GQ(T{0}, T{1}, T{0}, T{0});
  const auto gqj = GQ(T{0}, T{0}, T{1}, T{0});
  ExpectQuatEq(mEn::cross(MQ(gqi), MQ(gqj)), glm::cross(gqi, gqj));
}

TYPED_TEST(QuatGeometric, CrossIsNotCommutative) {
  using T = TypeParam;

  const auto gqi = GQ(T{0}, T{1}, T{0}, T{0});
  const auto gqj = GQ(T{0}, T{0}, T{1}, T{0});
  ExpectQuatEq(mEn::cross(MQ(gqi), MQ(gqj)), glm::cross(gqi, gqj));
  ExpectQuatEq(mEn::cross(MQ(gqj), MQ(gqi)), glm::cross(gqj, gqi));
}
