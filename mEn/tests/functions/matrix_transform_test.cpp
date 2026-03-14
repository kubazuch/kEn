#include <gtest/gtest.h>

#include <mEn/constants.hpp>
#include <mEn/functions/matrix_transform.hpp>

#include <glm/ext/matrix_transform.hpp>

#include "../util/mat_test_utils.hpp"
#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectMatEq;
using men_test::ExpectVecEq;
using men_test::GV;
using men_test::MM;
using men_test::MV;

template <typename T>
struct Mat4Transform : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Mat4Transform, TestedTypes);

}  // namespace

TYPED_TEST(Mat4Transform, IdentityMat3) {
  using T = TypeParam;

  ExpectMatEq(mEn::identity<3, T>(), glm::mat<3, 3, T>(T{1}));
}

TYPED_TEST(Mat4Transform, IdentityMat4) {
  using T = TypeParam;

  ExpectMatEq(mEn::identity<4, T>(), glm::mat<4, 4, T>(T{1}));
}

TYPED_TEST(Mat4Transform, TranslateOnIdentity) {
  using T = TypeParam;

  const auto v = GV<3, T>(T{1}, T{2}, T{3}, T{0});
  ExpectMatEq(mEn::translate(MM(glm::mat<4, 4, T>(T{1})), MV(v)), glm::translate(glm::mat<4, 4, T>(T{1}), v));
}

TYPED_TEST(Mat4Transform, TranslateComposes) {
  using T = TypeParam;

  const auto id = glm::mat<4, 4, T>(T{1});
  const auto v1 = GV<3, T>(T{1}, T{0}, T{0}, T{0});
  const auto v2 = GV<3, T>(T{0}, T{2}, T{0}, T{0});
  ExpectMatEq(mEn::translate(mEn::translate(MM(id), MV(v1)), MV(v2)), glm::translate(glm::translate(id, v1), v2));
}

TYPED_TEST(Mat4Transform, ScaleOnIdentity) {
  using T = TypeParam;

  const auto v = GV<3, T>(T{2}, T{3}, T{4}, T{0});
  ExpectMatEq(mEn::scale(MM(glm::mat<4, 4, T>(T{1})), MV(v)), glm::scale(glm::mat<4, 4, T>(T{1}), v));
}

TYPED_TEST(Mat4Transform, RotateByZeroIsIdentity) {
  using T = TypeParam;

  const auto axis = GV<3, T>(T{0}, T{0}, T{1}, T{0});
  ExpectMatEq(mEn::rotate(MM(glm::mat<4, 4, T>(T{1})), T{0}, MV(axis)),
              glm::rotate(glm::mat<4, 4, T>(T{1}), T{0}, axis));
}

TYPED_TEST(Mat4Transform, RotateHalfPiAroundZ) {
  using T = TypeParam;

  const auto axis = GV<3, T>(T{0}, T{0}, T{1}, T{0});
  ExpectMatEq(mEn::rotate(MM(glm::mat<4, 4, T>(T{1})), mEn::kHalfPi<T>, MV(axis)),
              glm::rotate(glm::mat<4, 4, T>(T{1}), mEn::kHalfPi<T>, axis));
}

TYPED_TEST(Mat4Transform, LookAtAlongNegativeZ) {
  using T = TypeParam;

  const auto eye    = GV<3, T>(T{0}, T{0}, T{5}, T{0});
  const auto center = GV<3, T>(T{0}, T{0}, T{0}, T{0});
  const auto up     = GV<3, T>(T{0}, T{1}, T{0}, T{0});
  ExpectMatEq(mEn::lookAt(MV(eye), MV(center), MV(up)), glm::lookAtRH(eye, center, up));
}

TYPED_TEST(Mat4Transform, ShearIdentityNoEffect) {
  using T = TypeParam;

  const auto id = glm::mat<4, 4, T>(T{1});
  const auto p  = GV<3, T>(T{1}, T{2}, T{3}, T{0});
  const auto z2 = GV<2, T>(T{0}, T{0}, T{0}, T{0});
  ExpectMatEq(mEn::shear(MM(id), MV(p), MV(z2), MV(z2), MV(z2)), glm::shear(id, p, z2, z2, z2));
}

TYPED_TEST(Mat4Transform, ShearXAxisOnly) {
  using T = TypeParam;

  const auto id  = glm::mat<4, 4, T>(T{1});
  const auto p   = GV<3, T>(T{0}, T{0}, T{0}, T{0});
  const auto l_x = GV<2, T>(T{2}, T{0}, T{0}, T{0});
  const auto z2  = GV<2, T>(T{0}, T{0}, T{0}, T{0});
  ExpectMatEq(mEn::shear(MM(id), MV(p), MV(l_x), MV(z2), MV(z2)), glm::shear(id, p, l_x, z2, z2));
}

TYPED_TEST(Mat4Transform, ShearAllAxes) {
  using T = TypeParam;

  const auto id  = glm::mat<4, 4, T>(T{1});
  const auto p   = GV<3, T>(T{1}, T{1}, T{1}, T{0});
  const auto l_x = GV<2, T>(T{1}, T{2}, T{0}, T{0});
  const auto l_y = GV<2, T>(T{3}, T{4}, T{0}, T{0});
  const auto l_z = GV<2, T>(T{5}, T{6}, T{0}, T{0});
  ExpectMatEq(mEn::shear(MM(id), MV(p), MV(l_x), MV(l_y), MV(l_z)), glm::shear(id, p, l_x, l_y, l_z));
}

TYPED_TEST(Mat4Transform, ShearXByYTransformsVector) {
  using T = TypeParam;

  const auto id  = glm::mat<4, 4, T>(T{1});
  const auto p   = GV<3, T>(T{0}, T{0}, T{0}, T{0});
  const auto l_x = GV<2, T>(T{3}, T{0}, T{0}, T{0});
  const auto z2  = GV<2, T>(T{0}, T{0}, T{0}, T{0});
  const auto v   = GV<4, T>(T{1}, T{2}, T{0}, T{1});
  ExpectVecEq(mEn::shear(MM(id), MV(p), MV(l_x), MV(z2), MV(z2)) * MV(v), glm::shear(id, p, l_x, z2, z2) * v);
}

TYPED_TEST(Mat4Transform, ShearYByXTransformsVector) {
  using T = TypeParam;

  const auto id  = glm::mat<4, 4, T>(T{1});
  const auto p   = GV<3, T>(T{0}, T{0}, T{0}, T{0});
  const auto l_y = GV<2, T>(T{2}, T{0}, T{0}, T{0});
  const auto z2  = GV<2, T>(T{0}, T{0}, T{0}, T{0});
  const auto v   = GV<4, T>(T{3}, T{1}, T{0}, T{1});
  ExpectVecEq(mEn::shear(MM(id), MV(p), MV(z2), MV(l_y), MV(z2)) * MV(v), glm::shear(id, p, z2, l_y, z2) * v);
}
