#include <gtest/gtest.h>

#include <cmath>

#include <mEn/constants.hpp>
#include <mEn/functions/matrix_projection.hpp>

#include <glm/ext/matrix_clip_space.hpp>

#include "../util/mat_test_utils.hpp"

namespace {

using men_test::ExpectMatEq;

template <typename T>
struct Mat4Projection : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Mat4Projection, TestedTypes);

}  // namespace

TYPED_TEST(Mat4Projection, OrthoSymmetric) {
  using T = TypeParam;

  ExpectMatEq(mEn::ortho(T{0}, T{2}, T{0}, T{2}, T{-1}, T{1}), glm::orthoRH_NO(T{0}, T{2}, T{0}, T{2}, T{-1}, T{1}));
}

TYPED_TEST(Mat4Projection, OrthoAsymmetric) {
  using T = TypeParam;

  ExpectMatEq(mEn::ortho(T{-5}, T{5}, T{-5}, T{5}, T{0}, T{10}),
              glm::orthoRH_NO(T{-5}, T{5}, T{-5}, T{5}, T{0}, T{10}));
}

TYPED_TEST(Mat4Projection, PerspectiveSquareAspect) {
  using T = TypeParam;

  const T near = T{0.1};
  const T far  = T{100};
  ExpectMatEq(mEn::perspective(mEn::kHalfPi<T>, T{1}, near, far),
              glm::perspectiveRH_NO(mEn::kHalfPi<T>, T{1}, near, far));
}

TYPED_TEST(Mat4Projection, PerspectiveAspectRatio) {
  using T = TypeParam;

  const T near = T{0.1};
  const T far  = T{100};
  ExpectMatEq(mEn::perspective(mEn::kHalfPi<T>, T{2}, near, far),
              glm::perspectiveRH_NO(mEn::kHalfPi<T>, T{2}, near, far));
}

TYPED_TEST(Mat4Projection, FrustumSymmetric) {
  using T = TypeParam;

  const T near = T{0.1};
  const T far  = T{100};
  ExpectMatEq(mEn::frustum(T{-0.2}, T{0.2}, T{-0.1}, T{0.1}, near, far),
              glm::frustumRH_NO(T{-0.2}, T{0.2}, T{-0.1}, T{0.1}, near, far));
}

TYPED_TEST(Mat4Projection, FrustumOffAxis) {
  using T = TypeParam;

  const T near = T{0.01};
  const T far  = T{100};
  ExpectMatEq(mEn::frustum(T{-0.0177} + T{0.0003}, T{0.0177} + T{0.0003}, T{-0.01}, T{0.01}, near, far),
              glm::frustumRH_NO(T{-0.0177} + T{0.0003}, T{0.0177} + T{0.0003}, T{-0.01}, T{0.01}, near, far));
}

TYPED_TEST(Mat4Projection, FrustumSymmetricMatchesPerspective) {
  using T = TypeParam;

  const T near   = T{0.1};
  const T far    = T{100};
  const T aspect = T{2};
  const T half_h = near * std::tan(mEn::kHalfPi<T> / T{2});
  const T half_w = half_h * aspect;
  ExpectMatEq(mEn::frustum(-half_w, half_w, -half_h, half_h, near, far),
              glm::perspectiveRH_NO(mEn::kHalfPi<T>, aspect, near, far));
}
