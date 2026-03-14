#pragma once

#include <gtest/gtest.h>

#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "../assert/mat3_eq.hpp"
#include "../assert/mat4_eq.hpp"

namespace men_test {

template <mEn::length_t N, typename T>
using M = mEn::mat<N, T>;

// Convert a glm::mat<N, N, T> to an mEn::mat<N, T>.
template <mEn::length_t N, typename T>
M<N, T> MM(const glm::mat<N, N, T>& g) {
  return M<N, T>(g);
}

// Dispatch to EXPECT_MAT3_EQ or EXPECT_MAT4_EQ.
template <mEn::length_t N, typename T>
void ExpectMatEq(const M<N, T>& actual, const glm::mat<N, N, T>& expected) {
  if constexpr (N == 3) {
    EXPECT_MAT3_EQ(actual, expected);
  } else {
    static_assert(N == 4);
    EXPECT_MAT4_EQ(actual, expected);
  }
}

}  // namespace men_test
