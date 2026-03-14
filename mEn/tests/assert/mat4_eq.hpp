#pragma once

#include <gtest/gtest.h>

#include <mEn/mat4.hpp>

#include <glm/mat4x4.hpp>

#include "mEn/fwd.hpp"
#include "scalar_eq.hpp"

template <class T>
::testing::AssertionResult Mat4ComponentsEqImpl(const char* m_expr, const mEn::mat<4, T>& m,
                                                const glm::mat<4, 4, T> e) {
  for (mEn::length_t c = 0; c < 4; ++c) {
    for (mEn::length_t r = 0; r < 4; ++r) {
      auto eq = ScalarEqImpl("m[c][r]", "e[c][r]", m[c][r], e[c][r]);
      if (!eq) {
        return ::testing::AssertionFailure()
               << "Mat4 component mismatch for " << m_expr << " (col=" << c << ", row=" << r << ")\n"
               << eq.message() << "\n"
               << "  " << m_expr << " = {"
               << "{" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "}, "
               << "{" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << "}, "
               << "{" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << "}, "
               << "{" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << "}"
               << "}\n"
               << "  expected = {"
               << "{" << e[0][0] << ", " << e[0][1] << ", " << e[0][2] << ", " << e[0][3] << "}, "
               << "{" << e[1][0] << ", " << e[1][1] << ", " << e[1][2] << ", " << e[1][3] << "}, "
               << "{" << e[2][0] << ", " << e[2][1] << ", " << e[2][2] << ", " << e[2][3] << "}, "
               << "{" << e[3][0] << ", " << e[3][1] << ", " << e[3][2] << ", " << e[3][3] << "}"
               << "}";
      }
    }
  }

  return ::testing::AssertionSuccess();
}

#define EXPECT_MAT4_EQ(m, e) EXPECT_TRUE(::Mat4ComponentsEqImpl(#m, (m), (e)))
