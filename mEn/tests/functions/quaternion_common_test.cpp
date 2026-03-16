#include <gtest/gtest.h>

#include <limits>

#include <mEn/constants.hpp>
#include <mEn/functions/quaternion_common.hpp>

#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "../util/quat_test_utils.hpp"
#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectBoolVecEq;
using men_test::ExpectQuatEq;
using men_test::GQ;
using men_test::MQ;
using men_test::Q;

template <typename T>
struct QuatCommon : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(QuatCommon, TestedTypes);

}  // namespace

TYPED_TEST(QuatCommon, LerpEndpoints) {
  using T = TypeParam;

  const auto gx = GQ(T{1}, T{0}, T{0}, T{0});
  const auto gy = GQ(T{0}, T{0}, T{0}, T{1});
  ExpectQuatEq(mEn::lerp(MQ(gx), MQ(gy), T{0}), glm::lerp(gx, gy, T{0}));
  ExpectQuatEq(mEn::lerp(MQ(gx), MQ(gy), T{1}), glm::lerp(gx, gy, T{1}));
}

TYPED_TEST(QuatCommon, LerpMidpoint) {
  using T = TypeParam;

  const auto gx = GQ(T{1}, T{2}, T{3}, T{4});
  const auto gy = GQ(T{5}, T{6}, T{7}, T{8});
  ExpectQuatEq(mEn::lerp(MQ(gx), MQ(gy), T{0.5}), glm::lerp(gx, gy, T{0.5}));
}

TYPED_TEST(QuatCommon, MixEndpoints) {
  using T = TypeParam;

  const auto gx = GQ(T{1}, T{0}, T{0}, T{0});
  const auto gy = GQ(T{0}, T{1}, T{0}, T{0});
  ExpectQuatEq(mEn::mix(MQ(gx), MQ(gy), T{0}), glm::mix(gx, gy, T{0}));
  ExpectQuatEq(mEn::mix(MQ(gx), MQ(gy), T{1}), glm::mix(gx, gy, T{1}));
}

TYPED_TEST(QuatCommon, SlerpEndpoints) {
  using T = TypeParam;

  const auto gx = GQ(T{1}, T{0}, T{0}, T{0});
  const auto gy = GQ(T{0}, T{1}, T{0}, T{0});
  ExpectQuatEq(mEn::slerp(MQ(gx), MQ(gy), T{0}), glm::slerp(gx, gy, T{0}));
  ExpectQuatEq(mEn::slerp(MQ(gx), MQ(gy), T{1}), glm::slerp(gx, gy, T{1}));
}

TYPED_TEST(QuatCommon, SlerpShortestArc) {
  using T = TypeParam;

  const auto gx    = GQ(T{1}, T{0}, T{0}, T{0});
  const auto g_neg = GQ(T{-1}, T{0}, T{0}, T{0});
  ExpectQuatEq(mEn::slerp(MQ(gx), MQ(g_neg), T{0.5}), glm::slerp(gx, g_neg, T{0.5}));
}

TYPED_TEST(QuatCommon, ConjugateNegatesXYZ) {
  using T = TypeParam;

  const auto gq = GQ(T{1}, T{2}, T{3}, T{4});
  ExpectQuatEq(mEn::conjugate(MQ(gq)), glm::conjugate(gq));
}

TYPED_TEST(QuatCommon, InverseOfUnitEqualsConjugate) {
  using T = TypeParam;

  const auto gq = glm::normalize(GQ(T{1}, T{0}, T{1}, T{0}));
  ExpectQuatEq(mEn::inverse(MQ(gq)), glm::conjugate(gq));
}

TYPED_TEST(QuatCommon, InverseGeneralCase) {
  using T = TypeParam;

  const auto gq = GQ(T{1}, T{2}, T{3}, T{4});
  ExpectQuatEq(MQ(gq) * mEn::inverse(MQ(gq)), glm::inverse(gq) * gq);
}

TYPED_TEST(QuatCommon, IsNanDetectsNaN) {
  using T = TypeParam;

  const auto gq       = GQ(T{1}, mEn::kNaN<T>, T{2}, T{3});
  const auto g_normal = GQ(T{1}, T{0}, T{0}, T{0});
  ExpectBoolVecEq(mEn::isnan(MQ(gq)), glm::isnan(gq));
  ExpectBoolVecEq(mEn::isnan(MQ(g_normal)), glm::isnan(g_normal));
}

TYPED_TEST(QuatCommon, IsInfDetectsInf) {
  using T = TypeParam;

  const T inf         = std::numeric_limits<T>::infinity();
  const auto gq       = GQ(T{1}, T{0}, inf, -inf);
  const auto g_normal = GQ(T{1}, T{0}, T{0}, T{0});
  ExpectBoolVecEq(mEn::isinf(MQ(gq)), glm::isinf(gq));
  ExpectBoolVecEq(mEn::isinf(MQ(g_normal)), glm::isinf(g_normal));
}
