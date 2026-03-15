#include <gtest/gtest.h>

#include <cstdint>
#include <format>

#include <mEn/fwd.hpp>
#include <mEn/features/format.hpp>  // NOLINT(misc-include-cleaner)
#include <mEn/functions/matrix_transform.hpp>

namespace {

template <typename T>
struct Format : ::testing::Test {};

using TestedTypes = ::testing::Types<std::int32_t, float, double>;
TYPED_TEST_SUITE(Format, TestedTypes);

}  // namespace

TYPED_TEST(Format, Vec2) {
  using T = TypeParam;

  EXPECT_EQ(std::format("{}", mEn::vec<2, T>{T{1}, T{2}}), "(1, 2)");
}

TYPED_TEST(Format, Vec3) {
  using T = TypeParam;

  EXPECT_EQ(std::format("{}", mEn::vec<3, T>{T{1}, T{2}, T{3}}), "(1, 2, 3)");
}

TYPED_TEST(Format, Vec4) {
  using T = TypeParam;

  EXPECT_EQ(std::format("{}", mEn::vec<4, T>{T{1}, T{2}, T{3}, T{4}}), "(1, 2, 3, 4)");
}

TYPED_TEST(Format, Mat3Identity) {
  using T = TypeParam;

  EXPECT_EQ(std::format("{}", mEn::identity<3, T>()), "((1, 0, 0), (0, 1, 0), (0, 0, 1))");
}

TYPED_TEST(Format, Mat4Identity) {
  using T = TypeParam;

  EXPECT_EQ(std::format("{}", mEn::identity<4, T>()), "((1, 0, 0, 0), (0, 1, 0, 0), (0, 0, 1, 0), (0, 0, 0, 1))");
}

TYPED_TEST(Format, Quat) {
  using T = TypeParam;

  EXPECT_EQ(std::format("{}", mEn::qua<T>{T{1}, T{2}, T{3}, T{4}}), "(1, 2, 3, 4)");
}
