#pragma once

#include <gtest/gtest.h>

#include <mEn/vec2.hpp>

#include <glm/vec2.hpp>

#include "scalar_eq.hpp"

template <class T>
::testing::AssertionResult Vec2ComponentsEqImpl(const char* v_expr, const mEn::vec<2, T>& v, const glm::vec<2, T>& e) {
  // Check x
  {
    auto r = ScalarEqImpl("v.x", "e.x", v.x, e.x);
    if (!r) {
      return ::testing::AssertionFailure() << "Vec2 component mismatch for " << v_expr << " (x)\n"
                                           << r.message() << "\n"
                                           << "  " << v_expr << " = {" << v.x << ", " << v.y << "}\n"
                                           << "  expected = {" << e.x << ", " << e.y << "}";
    }
  }

  // Check y
  {
    auto r = ScalarEqImpl("v.y", "e.y", v.y, e.y);
    if (!r) {
      return ::testing::AssertionFailure() << "Vec2 component mismatch for " << v_expr << " (y)\n"
                                           << r.message() << "\n"
                                           << "  " << v_expr << " = {" << v.x << ", " << v.y << "}\n"
                                           << "  expected = {" << e.x << ", " << e.y << "}";
    }
  }

  return ::testing::AssertionSuccess();
}

#define EXPECT_VEC2_EQ(v, e) EXPECT_TRUE(::Vec2ComponentsEqImpl(#v, (v), (e)))