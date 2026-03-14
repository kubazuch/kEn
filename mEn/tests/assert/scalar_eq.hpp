#pragma once

#include <gtest/gtest.h>

#include <mEn/config.hpp>
#include <mEn/constants.hpp>

template <class T>
::testing::AssertionResult ScalarEqImpl(const char* a_expr, const char* b_expr, T a, T b) {
  if constexpr (std::is_floating_point_v<T>) {
    const auto da   = static_cast<long double>(a);
    const auto db   = static_cast<long double>(b);
    const auto diff = std::fabsl(da - db);
    const auto eps  = static_cast<long double>(T{10} * mEn::kEpsilon<T>);

    if (diff <= eps) {
      return ::testing::AssertionSuccess();
    }

    return ::testing::AssertionFailure() << "Expected " << a_expr << " ~= " << b_expr << "\n"
                                         << "  " << a_expr << " = " << a << "\n"
                                         << "  " << b_expr << " = " << b << "\n"
                                         << "  |diff| = " << static_cast<double>(diff)
                                         << " > eps = " << static_cast<double>(eps);
  } else {
    if (a == b) {
      return ::testing::AssertionSuccess();
    }

    return ::testing::AssertionFailure() << "Expected " << a_expr << " == " << b_expr << "\n"
                                         << "  " << a_expr << " = " << a << "\n"
                                         << "  " << b_expr << " = " << b;
  }
}

#define EXPECT_SCALAR_EQ(a, b) EXPECT_TRUE(::ScalarEqImpl(#a, #b, (a), (b)))