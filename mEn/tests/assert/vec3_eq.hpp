#pragma once

#include <gtest/gtest.h>

#include <mEn/vec3.hpp>

#include <glm/vec3.hpp>

#include "scalar_eq.hpp"

template <class T>
::testing::AssertionResult Vec3ComponentsEqImpl(const char* v_expr, const mEn::vec<3, T>& v, const glm::vec<3, T>& e) {
  // Check x
  {
    auto r = ScalarEqImpl("v.x", "e.x", v.x, e.x);
    if (!r) {
      return ::testing::AssertionFailure() << "Vec3 component mismatch for " << v_expr << " (x)\n"
                                           << r.message() << "\n"
                                           << "  " << v_expr << " = {" << v.x << ", " << v.y << ", " << v.z << "}\n"
                                           << "  expected = {" << e.x << ", " << e.y << ", " << e.z << "}";
    }
  }

  // Check y
  {
    auto r = ScalarEqImpl("v.y", "e.y", v.y, e.y);
    if (!r) {
      return ::testing::AssertionFailure() << "Vec3 component mismatch for " << v_expr << " (y)\n"
                                           << r.message() << "\n"
                                           << "  " << v_expr << " = {" << v.x << ", " << v.y << ", " << v.z << "}\n"
                                           << "  expected = {" << e.x << ", " << e.y << ", " << e.z << "}";
    }
  }

  // Check z
  {
    auto r = ScalarEqImpl("v.z", "e.z", v.z, e.z);
    if (!r) {
      return ::testing::AssertionFailure() << "Vec3 component mismatch for " << v_expr << " (z)\n"
                                           << r.message() << "\n"
                                           << "  " << v_expr << " = {" << v.x << ", " << v.y << ", " << v.z << "}\n"
                                           << "  expected = {" << e.x << ", " << e.y << ", " << e.z << "}";
    }
  }

  return ::testing::AssertionSuccess();
}

#define EXPECT_VEC3_EQ(v, e) EXPECT_TRUE(::Vec3ComponentsEqImpl(#v, (v), (e)))