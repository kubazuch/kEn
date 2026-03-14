#include <gtest/gtest.h>

#include <cmath>

#include <mEn/constants.hpp>
#include <mEn/functions/quaternion_exponential.hpp>

#include <glm/ext/quaternion_exponential.hpp>

#include "../util/quat_test_utils.hpp"

namespace {

using men_test::ExpectQuatEq;
using men_test::GQ;
using men_test::MQ;
using men_test::Q;

template <typename T>
struct QuatExponential : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(QuatExponential, TestedTypes);

}  // namespace

TYPED_TEST(QuatExponential, ExpOfPureZeroIsIdentity) {
  using T = TypeParam;

  // NOTE: glm::exp is NOT used as oracle here. glm::exp(quat) only handles pure
  // quaternions (ignores the w component), while mEn::exp computes the full
  // quaternion exponential: exp(w + u) = exp(w) * (cos|u| + sin|u|*u/|u|).
  const auto g_zero = GQ(T{0}, T{0}, T{0}, T{0});
  const auto g_id   = GQ(T{1}, T{0}, T{0}, T{0});
  ExpectQuatEq(mEn::exp(MQ(g_zero)), g_id);
}

TYPED_TEST(QuatExponential, LogOfIdentityIsZero) {
  using T = TypeParam;

  const auto gid = GQ(T{1}, T{0}, T{0}, T{0});
  ExpectQuatEq(mEn::log(MQ(gid)), glm::log(gid));
}

TYPED_TEST(QuatExponential, PowWithExponentOne) {
  using T = TypeParam;

  // 90-deg rotation around Z: w=cos(pi/4), z=sin(pi/4)
  const T half_angle = mEn::kPi<T> / T{4};
  const auto gq      = GQ(std::cos(half_angle), T{0}, T{0}, std::sin(half_angle));
  ExpectQuatEq(mEn::pow(MQ(gq), T{1}), glm::pow(gq, T{1}));
}

TYPED_TEST(QuatExponential, PowWithExponentZeroIsIdentity) {
  using T = TypeParam;

  const T half_angle = mEn::kPi<T> / T{4};
  const auto gq      = GQ(std::cos(half_angle), T{0}, T{0}, std::sin(half_angle));
  ExpectQuatEq(mEn::pow(MQ(gq), T{0}), glm::pow(gq, T{0}));
}

TYPED_TEST(QuatExponential, SqrtSquaredIsOriginal) {
  using T = TypeParam;

  // 45-deg rotation around Y
  const T half_angle = mEn::kPi<T> / T{8};
  const auto gq      = GQ(std::cos(half_angle), T{0}, std::sin(half_angle), T{0});
  ExpectQuatEq(mEn::sqrt(MQ(gq)) * mEn::sqrt(MQ(gq)), glm::sqrt(gq) * glm::sqrt(gq));
}

TYPED_TEST(QuatExponential, ExpLogRoundtrip) {
  using T = TypeParam;

  // 60-deg rotation around X
  // NOTE: glm::exp(glm::log(gq)) is NOT used as oracle -- see note above ExpOfPureZeroIsIdentity.
  // The expected result is the original quaternion (mathematical identity for unit quaternions).
  const T half_angle = mEn::kPi<T> / T{6};
  const auto gq      = GQ(std::cos(half_angle), std::sin(half_angle), T{0}, T{0});
  ExpectQuatEq(mEn::exp(mEn::log(MQ(gq))), gq);
}
