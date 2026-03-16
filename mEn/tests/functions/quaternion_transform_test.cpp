#include <gtest/gtest.h>

#include <mEn/constants.hpp>
#include <mEn/functions/quaternion_geometric.hpp>
#include <mEn/functions/quaternion_transform.hpp>

#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../assert/scalar_eq.hpp"
#include "../util/quat_test_utils.hpp"
#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectQuatEq;
using men_test::ExpectVecEq;
using men_test::GQ;
using men_test::GV;
using men_test::MV;

template <typename T>
struct QuatTransform : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(QuatTransform, TestedTypes);

}  // namespace

TYPED_TEST(QuatTransform, IdentityHasWOneXYZZero) {
  using T = TypeParam;

  ExpectQuatEq(mEn::identity<T>(), GQ(T{1}, T{0}, T{0}, T{0}));
}

TYPED_TEST(QuatTransform, IdentityIsUnitLength) {
  using T = TypeParam;

  EXPECT_SCALAR_EQ(mEn::length(mEn::identity<T>()), T{1});
}

TYPED_TEST(QuatTransform, RotateIdentityByZeroIsIdentity) {
  using T = TypeParam;

  const auto g_id   = GQ(T{1}, T{0}, T{0}, T{0});
  const auto g_axis = GV<3, T>(T{0}, T{0}, T{1}, T{0});
  ExpectQuatEq(mEn::rotate(mEn::identity<T>(), T{0}, MV(g_axis)), glm::rotate(g_id, T{0}, g_axis));
}

TYPED_TEST(QuatTransform, RotateIdentityHalfPiAroundZ) {
  using T = TypeParam;

  const auto g_id   = GQ(T{1}, T{0}, T{0}, T{0});
  const auto g_axis = GV<3, T>(T{0}, T{0}, T{1}, T{0});
  ExpectQuatEq(mEn::rotate(mEn::identity<T>(), mEn::kHalfPi<T>, MV(g_axis)),
               glm::rotate(g_id, mEn::kHalfPi<T>, g_axis));
}

TYPED_TEST(QuatTransform, RotateResultIsUnitLength) {
  using T = TypeParam;

  const auto g_axis = GV<3, T>(T{1}, T{0}, T{0}, T{0});
  EXPECT_SCALAR_EQ(mEn::length(mEn::rotate(mEn::identity<T>(), mEn::kPi<T> / T{3}, MV(g_axis))), T{1});
}

TYPED_TEST(QuatTransform, RotateAccumulatesRotation) {
  using T = TypeParam;

  // Two successive 90-degree rotations around Z should equal one 180-degree rotation around Z.
  const auto g_id   = GQ(T{1}, T{0}, T{0}, T{0});
  const auto g_axis = GV<3, T>(T{0}, T{0}, T{1}, T{0});
  const auto q90    = mEn::rotate(mEn::identity<T>(), mEn::kHalfPi<T>, MV(g_axis));
  const auto g_q90  = glm::rotate(g_id, mEn::kHalfPi<T>, g_axis);
  ExpectQuatEq(mEn::rotate(q90, mEn::kHalfPi<T>, MV(g_axis)), glm::rotate(g_q90, mEn::kHalfPi<T>, g_axis));
}

TYPED_TEST(QuatTransform, QuatLookAtIsUnitLength) {
  using T = TypeParam;

  const auto g_dir = GV<3, T>(T{0}, T{0}, T{-1}, T{0});
  const auto g_up  = GV<3, T>(T{0}, T{1}, T{0}, T{0});
  EXPECT_SCALAR_EQ(mEn::length(mEn::quatLookAt(MV(g_dir), MV(g_up))), T{1});
}

TYPED_TEST(QuatTransform, QuatLookAtMatchesGLM) {
  using T = TypeParam;

  const auto g_dir = GV<3, T>(T{0}, T{0}, T{-1}, T{0});
  const auto g_up  = GV<3, T>(T{0}, T{1}, T{0}, T{0});
  ExpectQuatEq(mEn::quatLookAt(MV(g_dir), MV(g_up)), glm::quatLookAtRH(g_dir, g_up));
}

TYPED_TEST(QuatTransform, QuatLookAtFacesTarget) {
  using T = TypeParam;

  // Camera looking along (-1,0,0), world-up = Y.
  // In RH convention, the camera's local -Z axis maps to the look direction.
  const auto g_dir      = GV<3, T>(T{-1}, T{0}, T{0}, T{0});
  const auto g_up       = GV<3, T>(T{0}, T{1}, T{0}, T{0});
  const auto g_local_nz = GV<3, T>(T{0}, T{0}, T{-1}, T{0});

  const auto q       = mEn::quatLookAt(MV(g_dir), MV(g_up));
  const auto rotated = q * MV(g_local_nz);
  ExpectVecEq(rotated, g_dir);
}
