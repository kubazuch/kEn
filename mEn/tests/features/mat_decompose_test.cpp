#include <gtest/gtest.h>

#include <mEn/constants.hpp>
#include <mEn/features/mat_decompose.hpp>
#include <mEn/functions/matrix_transform.hpp>

#include <glm/ext/quaternion_trigonometric.hpp>

#include "../util/quat_test_utils.hpp"
#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectQuatEq;
using men_test::ExpectVecEq;
using men_test::GQ;
using men_test::GV;

template <typename T>
struct MatDecompose : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(MatDecompose, TestedTypes);

}  // namespace

TYPED_TEST(MatDecompose, IdentityMatrix) {
  using T = TypeParam;

  const auto id = mEn::identity<4, T>();
  mEn::vec<3, T> t{};
  mEn::qua<T> r{};
  mEn::vec<3, T> s{};

  const bool ok = mEn::decompose(id, t, r, s);

  EXPECT_TRUE(ok);
  ExpectVecEq(t, GV<3, T>(T{0}, T{0}, T{0}, T{0}));
  ExpectVecEq(s, GV<3, T>(T{1}, T{1}, T{1}, T{0}));
  ExpectQuatEq(r, GQ(T{1}, T{0}, T{0}, T{0}));
}

TYPED_TEST(MatDecompose, TranslationOnly) {
  using T = TypeParam;

  const auto id = mEn::identity<4, T>();
  const auto m  = mEn::translate(id, mEn::vec<3, T>{T{1}, T{2}, T{3}});

  mEn::vec<3, T> t{};
  mEn::qua<T> r{};
  mEn::vec<3, T> s{};

  const bool ok = mEn::decompose(m, t, r, s);

  EXPECT_TRUE(ok);
  ExpectVecEq(t, GV<3, T>(T{1}, T{2}, T{3}, T{0}));
  ExpectVecEq(s, GV<3, T>(T{1}, T{1}, T{1}, T{0}));
  ExpectQuatEq(r, GQ(T{1}, T{0}, T{0}, T{0}));
}

TYPED_TEST(MatDecompose, ScaleOnly) {
  using T = TypeParam;

  const auto id = mEn::identity<4, T>();
  const auto m  = mEn::scale(id, mEn::vec<3, T>{T{2}, T{3}, T{4}});

  mEn::vec<3, T> t{};
  mEn::qua<T> r{};
  mEn::vec<3, T> s{};

  const bool ok = mEn::decompose(m, t, r, s);

  EXPECT_TRUE(ok);
  ExpectVecEq(t, GV<3, T>(T{0}, T{0}, T{0}, T{0}));
  ExpectVecEq(s, GV<3, T>(T{2}, T{3}, T{4}, T{0}));
  ExpectQuatEq(r, GQ(T{1}, T{0}, T{0}, T{0}));
}

TYPED_TEST(MatDecompose, RotationOnly) {
  using T = TypeParam;

  // 90 degrees around Y axis
  const auto axis_y  = mEn::vec<3, T>{T{0}, T{1}, T{0}};
  const auto rot_mat = mEn::rotate(mEn::identity<4, T>(), mEn::kHalfPi<T>, axis_y);

  mEn::vec<3, T> t{};
  mEn::qua<T> r{};
  mEn::vec<3, T> s{};

  const bool ok = mEn::decompose(rot_mat, t, r, s);

  EXPECT_TRUE(ok);
  ExpectVecEq(t, GV<3, T>(T{0}, T{0}, T{0}, T{0}));
  ExpectVecEq(s, GV<3, T>(T{1}, T{1}, T{1}, T{0}));
  ExpectQuatEq(r, glm::angleAxis(mEn::kHalfPi<T>, glm::vec<3, T>(T{0}, T{1}, T{0})));
}

TYPED_TEST(MatDecompose, TranslateScaleRotateCombined) {
  using T = TypeParam;

  // Build T * R * S
  const auto id     = mEn::identity<4, T>();
  const auto t_mat  = mEn::translate(id, mEn::vec<3, T>{T{5}, T{-3}, T{2}});
  const T angle     = mEn::kPi<T> / T{4};  // 45 degrees
  const auto axis_z = mEn::vec<3, T>{T{0}, T{0}, T{1}};
  const auto tr_mat = mEn::rotate(t_mat, angle, axis_z);
  const auto trs    = mEn::scale(tr_mat, mEn::vec<3, T>{T{2}, T{2}, T{2}});

  mEn::vec<3, T> t{};
  mEn::qua<T> r{};
  mEn::vec<3, T> s{};

  const bool ok = mEn::decompose(trs, t, r, s);

  EXPECT_TRUE(ok);
  ExpectVecEq(t, GV<3, T>(T{5}, T{-3}, T{2}, T{0}));
  ExpectVecEq(s, GV<3, T>(T{2}, T{2}, T{2}, T{0}));
  ExpectQuatEq(r, glm::angleAxis(angle, glm::vec<3, T>(T{0}, T{0}, T{1})));
}

TYPED_TEST(MatDecompose, FailsOnPerspectiveMatrix) {
  using T = TypeParam;

  // Set a non-zero perspective row (row 3, cols 0-2)
  auto m  = mEn::identity<4, T>();
  m[0][3] = T{1};  // m[col][row]: col=0, row=3

  mEn::vec<3, T> t{};
  mEn::qua<T> r{};
  mEn::vec<3, T> s{};

  const bool ok = mEn::decompose(m, t, r, s);

  EXPECT_FALSE(ok);
}

TYPED_TEST(MatDecompose, FailsOnZeroScaleMatrix) {
  using T = TypeParam;

  // Zero out the first column to produce a zero-scale X axis
  const auto id = mEn::identity<4, T>();
  const auto m  = mEn::scale(id, mEn::vec<3, T>{T{0}, T{1}, T{1}});

  mEn::vec<3, T> t{};
  mEn::qua<T> r{};
  mEn::vec<3, T> s{};

  const bool ok = mEn::decompose(m, t, r, s);

  EXPECT_FALSE(ok);
}
