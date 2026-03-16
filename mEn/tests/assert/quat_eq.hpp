#pragma once

#include <gtest/gtest.h>

#include <mEn/quat.hpp>

#include <glm/detail/type_quat.hpp>

#include "scalar_eq.hpp"

template <class T>
::testing::AssertionResult QuatComponentsEqImpl(const char* q_expr, const mEn::qua<T>& q, const glm::vec<4, T>& e) {
  // Check x
  {
    auto r = ScalarEqImpl("q.x", "e.x", q.x, e.x);
    if (!r) {
      return ::testing::AssertionFailure()
             << "Quat component mismatch for " << q_expr << " (x)\n"
             << r.message() << "\n"
             << "  " << q_expr << " = {" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "}\n"
             << "  expected = {" << e.x << ", " << e.y << ", " << e.z << ", " << e.w << "}";
    }
  }

  // Check y
  {
    auto r = ScalarEqImpl("q.y", "e.y", q.y, e.y);
    if (!r) {
      return ::testing::AssertionFailure()
             << "Quat component mismatch for " << q_expr << " (y)\n"
             << r.message() << "\n"
             << "  " << q_expr << " = {" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "}\n"
             << "  expected = {" << e.x << ", " << e.y << ", " << e.z << ", " << e.w << "}";
    }
  }

  // Check z
  {
    auto r = ScalarEqImpl("q.z", "e.z", q.z, e.z);
    if (!r) {
      return ::testing::AssertionFailure()
             << "Quat component mismatch for " << q_expr << " (z)\n"
             << r.message() << "\n"
             << "  " << q_expr << " = {" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "}\n"
             << "  expected = {" << e.x << ", " << e.y << ", " << e.z << ", " << e.w << "}";
    }
  }

  // Check w
  {
    auto r = ScalarEqImpl("q.w", "e.w", q.w, e.w);
    if (!r) {
      return ::testing::AssertionFailure()
             << "Quat component mismatch for " << q_expr << " (w)\n"
             << r.message() << "\n"
             << "  " << q_expr << " = {" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "}\n"
             << "  expected = {" << e.x << ", " << e.y << ", " << e.z << ", " << e.w << "}";
    }
  }

  return ::testing::AssertionSuccess();
}

#define EXPECT_QUAT_EQ(q, e) EXPECT_TRUE(::QuatComponentsEqImpl(#q, (q), (e)))