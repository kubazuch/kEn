#pragma once

#include <gtest/gtest.h>

#include <mEn/mat3.hpp>

#include <glm/mat3x3.hpp>

#include "scalar_eq.hpp"

template <class T>
::testing::AssertionResult Mat3ComponentsEqImpl(const char* m_expr, const mEn::mat<3, T>& m,
                                                const glm::mat<3, 3, T>& e) {
  for (mEn::length_t c = 0; c < 3; ++c) {
    for (mEn::length_t r = 0; r < 3; ++r) {
      auto eq = ScalarEqImpl("m[c][r]", "e[c][r]", m[c][r], e[c][r]);
      if (!eq) {
        return ::testing::AssertionFailure()
               << "Mat3 component mismatch for " << m_expr << " (col=" << c << ", row=" << r << ")\n"
               << eq.message() << "\n"
               << "  " << m_expr << " = {"
               << "{" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << "}, "
               << "{" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << "}, "
               << "{" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << "}"
               << "}\n"
               << "  expected = {"
               << "{" << e[0][0] << ", " << e[0][1] << ", " << e[0][2] << "}, "
               << "{" << e[1][0] << ", " << e[1][1] << ", " << e[1][2] << "}, "
               << "{" << e[2][0] << ", " << e[2][1] << ", " << e[2][2] << "}"
               << "}";
      }
    }
  }

  return ::testing::AssertionSuccess();
}

#define EXPECT_MAT3_EQ(m, e) EXPECT_TRUE(::Mat3ComponentsEqImpl(#m, (m), (e)))
