#pragma once

#include <gtest/gtest.h>

#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>

#include "../assert/vec2_eq.hpp"
#include "../assert/vec3_eq.hpp"
#include "../assert/vec4_eq.hpp"

namespace men_test {

template <mEn::length_t L, typename T>
using V = mEn::vec<L, T>;

// Build a glm::vec<L, T> from 4 scalars, using the first L components.
template <glm::length_t L, typename T>
glm::vec<L, T> GV(T a, T b, T c, T d) {
  if constexpr (L == 2) {
    return glm::vec<2, T>{a, b};
  } else if constexpr (L == 3) {
    return glm::vec<3, T>{a, b, c};
  } else {
    static_assert(L == 4);
    return glm::vec<4, T>{a, b, c, d};
  }
}

// Convert a glm::vec<L, T> to an mEn::vec<L, T>.
template <glm::length_t L, typename T>
V<mEn::length_t{L}, T> MV(const glm::vec<L, T>& g) {
  return V<mEn::length_t{L}, T>(g);
}

template <mEn::length_t L, typename T>
void ExpectVecEq(const V<L, T>& actual, const glm::vec<glm::length_t{L}, T>& expected) {
  if constexpr (L == 2) {
    EXPECT_VEC2_EQ(actual, expected);
  } else if constexpr (L == 3) {
    EXPECT_VEC3_EQ(actual, expected);
  } else {
    static_assert(L == 4);
    EXPECT_VEC4_EQ(actual, expected);
  }
}

template <mEn::length_t L>
void ExpectBoolVecEq(const mEn::vec<L, bool>& actual, const glm::vec<glm::length_t{L}, bool>& expected) {
  for (mEn::length_t i = 0; i < L; ++i) {
    EXPECT_EQ(actual[i], expected[i]) << "component " << i;
  }
}

}  // namespace men_test
