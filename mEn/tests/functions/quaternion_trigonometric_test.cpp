#include <gtest/gtest.h>

#include <mEn/constants.hpp>
#include <mEn/functions/quaternion_trigonometric.hpp>

#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../assert/scalar_eq.hpp"
#include "../util/quat_test_utils.hpp"
#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectQuatEq;
using men_test::ExpectVecEq;
using men_test::GQ;
using men_test::GV;
using men_test::MQ;
using men_test::MV;

template <typename T>
struct QuatTrigonometric : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(QuatTrigonometric, TestedTypes);

}  // namespace

TYPED_TEST(QuatTrigonometric, AngleAxisMatchesGLM) {
  using T = TypeParam;

  const auto g_ax = GV<3, T>(T{1}, T{0}, T{0}, T{0});
  ExpectQuatEq(mEn::angleAxis(mEn::kHalfPi<T>, MV(g_ax)), glm::angleAxis(mEn::kHalfPi<T>, g_ax));
}

TYPED_TEST(QuatTrigonometric, AngleAxisRoundtrip) {
  using T = TypeParam;

  const T a       = mEn::kPi<T> / T{3};
  const auto g_ax = GV<3, T>(T{1}, T{0}, T{0}, T{0});
  const auto gq   = glm::angleAxis(a, g_ax);

  EXPECT_SCALAR_EQ(mEn::angle(mEn::angleAxis(a, MV(g_ax))), glm::angle(gq));
  ExpectVecEq(mEn::axis(mEn::angleAxis(a, MV(g_ax))), glm::axis(gq));
}

TYPED_TEST(QuatTrigonometric, AngleOfIdentityIsZero) {
  using T = TypeParam;

  const auto g_id = GQ(T{1}, T{0}, T{0}, T{0});
  EXPECT_SCALAR_EQ(mEn::angle(MQ(g_id)), glm::angle(g_id));
}

TYPED_TEST(QuatTrigonometric, EulerAnglesMatchesGLM) {
  using T = TypeParam;

  // 45-deg rotation around X
  const auto g_ax = GV<3, T>(T{1}, T{0}, T{0}, T{0});
  const auto gq   = glm::angleAxis(mEn::kPi<T> / T{4}, g_ax);
  ExpectVecEq(mEn::eulerAngles(MQ(gq)), glm::eulerAngles(gq));
}

TYPED_TEST(QuatTrigonometric, RollPitchYawMatchEulerAngles) {
  using T = TypeParam;

  const auto g_ax = GV<3, T>(T{1}, T{0}, T{0}, T{0});
  const auto gq   = glm::angleAxis(mEn::kPi<T> / T{6}, g_ax);

  EXPECT_SCALAR_EQ(mEn::pitch(MQ(gq)), glm::pitch(gq));
  EXPECT_SCALAR_EQ(mEn::yaw(MQ(gq)), glm::yaw(gq));
  EXPECT_SCALAR_EQ(mEn::roll(MQ(gq)), glm::roll(gq));
}

TYPED_TEST(QuatTrigonometric, PurePitchRotation) {
  using T = TypeParam;

  const auto g_ax = GV<3, T>(T{1}, T{0}, T{0}, T{0});
  const auto gq   = glm::angleAxis(mEn::kPi<T> / T{4}, g_ax);

  EXPECT_SCALAR_EQ(mEn::pitch(MQ(gq)), glm::pitch(gq));
  EXPECT_SCALAR_EQ(mEn::yaw(MQ(gq)), glm::yaw(gq));
  EXPECT_SCALAR_EQ(mEn::roll(MQ(gq)), glm::roll(gq));
}

TYPED_TEST(QuatTrigonometric, PureYawRotation) {
  using T = TypeParam;

  const auto g_ax = GV<3, T>(T{0}, T{1}, T{0}, T{0});
  const auto gq   = glm::angleAxis(mEn::kPi<T> / T{4}, g_ax);

  EXPECT_SCALAR_EQ(mEn::yaw(MQ(gq)), glm::yaw(gq));
  EXPECT_SCALAR_EQ(mEn::pitch(MQ(gq)), glm::pitch(gq));
  EXPECT_SCALAR_EQ(mEn::roll(MQ(gq)), glm::roll(gq));
}

TYPED_TEST(QuatTrigonometric, PureRollRotation) {
  using T = TypeParam;

  const auto g_ax = GV<3, T>(T{0}, T{0}, T{1}, T{0});
  const auto gq   = glm::angleAxis(mEn::kPi<T> / T{4}, g_ax);

  EXPECT_SCALAR_EQ(mEn::roll(MQ(gq)), glm::roll(gq));
  EXPECT_SCALAR_EQ(mEn::pitch(MQ(gq)), glm::pitch(gq));
  EXPECT_SCALAR_EQ(mEn::yaw(MQ(gq)), glm::yaw(gq));
}
