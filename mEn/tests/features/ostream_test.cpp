#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>

#include <mEn/features/ostream.hpp>  //NOLINT(misc-include-cleaner)
#include <mEn/functions/matrix_transform.hpp>
#include <mEn/fwd.hpp>

namespace {

template <typename T>
struct Ostream : ::testing::Test {};

using TestedTypes = ::testing::Types<std::int32_t, float, double>;
TYPED_TEST_SUITE(Ostream, TestedTypes);

}  // namespace

TYPED_TEST(Ostream, Vec2) {
  using T = TypeParam;

  std::ostringstream ss;
  ss << mEn::vec<2, T>{T{1}, T{2}};
  EXPECT_EQ(ss.str(), "(1, 2)");
}

TYPED_TEST(Ostream, Vec3) {
  using T = TypeParam;

  std::ostringstream ss;
  ss << mEn::vec<3, T>{T{1}, T{2}, T{3}};
  EXPECT_EQ(ss.str(), "(1, 2, 3)");
}

TYPED_TEST(Ostream, Vec4) {
  using T = TypeParam;

  std::ostringstream ss;
  ss << mEn::vec<4, T>{T{1}, T{2}, T{3}, T{4}};
  EXPECT_EQ(ss.str(), "(1, 2, 3, 4)");
}

TYPED_TEST(Ostream, Mat3Identity) {
  using T = TypeParam;

  std::ostringstream ss;
  ss << mEn::identity<3, T>();
  EXPECT_EQ(ss.str(), "((1, 0, 0), (0, 1, 0), (0, 0, 1))");
}

TYPED_TEST(Ostream, Mat4Identity) {
  using T = TypeParam;

  std::ostringstream ss;
  ss << mEn::identity<4, T>();
  EXPECT_EQ(ss.str(), "((1, 0, 0, 0), (0, 1, 0, 0), (0, 0, 1, 0), (0, 0, 0, 1))");
}

TYPED_TEST(Ostream, Quat) {
  using T = TypeParam;

  std::ostringstream ss;
  ss << mEn::qua<T>{T{1}, T{2}, T{3}, T{4}};
  EXPECT_EQ(ss.str(), "(1, 2, 3, 4)");
}
