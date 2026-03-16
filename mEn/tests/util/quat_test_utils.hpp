#pragma once

#include <gtest/gtest.h>

#include <mEn/quat.hpp>

#include <glm/detail/type_quat.hpp>
#include <glm/vec4.hpp>

#include "../assert/quat_eq.hpp"

namespace men_test {

template <typename T>
using Q = mEn::qua<T>;

// Build a glm::qua<T> from (w, x, y, z) - matching mEn's constructor parameter order.
template <typename T>
glm::qua<T, glm::defaultp> GQ(T w, T x, T y, T z) {
  return glm::qua<T, glm::defaultp>(w, x, y, z);
}

// Convert a glm::qua<T> to an mEn::qua<T>.
template <typename T>
Q<T> MQ(const glm::qua<T, glm::defaultp>& g) {
  return Q<T>(g);
}

template <typename T>
void ExpectQuatEq(const Q<T>& actual, const glm::qua<T, glm::defaultp>& expected) {
  EXPECT_QUAT_EQ(actual, (glm::vec<4, T>{expected.x, expected.y, expected.z, expected.w}));
}

}  // namespace men_test
