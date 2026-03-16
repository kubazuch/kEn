#pragma once

#include <gtest/gtest.h>

#include <mEn/vec4.hpp>

#include <glm/vec4.hpp>

#include "scalar_eq.hpp"

template <class T>
::testing::AssertionResult Vec4ComponentsEqImpl(const char* v_expr, const mEn::vec<4, T>& v, const glm::vec<4, T>& e) {
  // Check x
  {
    auto r = ScalarEqImpl("v.x", "e.x", v.x, e.x);
    if (!r) {
      return ::testing::AssertionFailure()
             << "Vec4 component mismatch for " << v_expr << " (x)\n"
             << r.message() << "\n"
             << "  " << v_expr << " = {" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}\n"
             << "  expected = {" << e.x << ", " << e.y << ", " << e.z << ", " << e.w << "}";
    }
  }

  // Check y
  {
    auto r = ScalarEqImpl("v.y", "e.y", v.y, e.y);
    if (!r) {
      return ::testing::AssertionFailure()
             << "Vec4 component mismatch for " << v_expr << " (y)\n"
             << r.message() << "\n"
             << "  " << v_expr << " = {" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}\n"
             << "  expected = {" << e.x << ", " << e.y << ", " << e.z << ", " << e.w << "}";
    }
  }

  // Check z
  {
    auto r = ScalarEqImpl("v.z", "e.z", v.z, e.z);
    if (!r) {
      return ::testing::AssertionFailure()
             << "Vec4 component mismatch for " << v_expr << " (z)\n"
             << r.message() << "\n"
             << "  " << v_expr << " = {" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}\n"
             << "  expected = {" << e.x << ", " << e.y << ", " << e.z << ", " << e.w << "}";
    }
  }

  // Check w
  {
    auto r = ScalarEqImpl("v.w", "e.w", v.w, e.w);
    if (!r) {
      return ::testing::AssertionFailure()
             << "Vec4 component mismatch for " << v_expr << " (w)\n"
             << r.message() << "\n"
             << "  " << v_expr << " = {" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}\n"
             << "  expected = {" << e.x << ", " << e.y << ", " << e.z << ", " << e.w << "}";
    }
  }

  return ::testing::AssertionSuccess();
}

#define EXPECT_VEC4_EQ(v, e) EXPECT_TRUE(::Vec4ComponentsEqImpl(#v, (v), (e)))