#include <gtest/gtest.h>

#include <mEn/features/type_ptr.hpp>

#include "../assert/mat3_eq.hpp"
#include "../assert/mat4_eq.hpp"
#include "../assert/quat_eq.hpp"
#include "../assert/vec2_eq.hpp"
#include "../assert/vec3_eq.hpp"
#include "../assert/vec4_eq.hpp"
#include "../util/test_utils.hpp"

namespace {

template <typename T>
struct TypePtr : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(TypePtr, TestedTypes);

}  // namespace

TYPED_TEST(TypePtr, ValuePtrVec2PointsToX) {
  using T = TypeParam;

  mEn::vec<2, T> v{T{1}, T{2}};
  EXPECT_EQ(mEn::value_ptr(v), &v.x);
}

TYPED_TEST(TypePtr, ValuePtrVec3PointsToX) {
  using T = TypeParam;

  mEn::vec<3, T> v{T{1}, T{2}, T{3}};
  EXPECT_EQ(mEn::value_ptr(v), &v.x);
}

TYPED_TEST(TypePtr, ValuePtrVec4PointsToX) {
  using T = TypeParam;

  mEn::vec<4, T> v{T{1}, T{2}, T{3}, T{4}};
  EXPECT_EQ(mEn::value_ptr(v), &v.x);
}

TYPED_TEST(TypePtr, ValuePtrMat3PointsToFirstColumnX) {
  using T = TypeParam;

  const mEn::mat<3, T> m{T{1}, T{0}, T{0}, T{0}, T{1}, T{0}, T{0}, T{0}, T{1}};
  EXPECT_EQ(mEn::value_ptr(m), &m[0].x);
}

TYPED_TEST(TypePtr, ValuePtrMat4PointsToFirstColumnX) {
  using T = TypeParam;

  const mEn::mat<4, T> m{T{1}, T{0}, T{0}, T{0}, T{0}, T{1}, T{0}, T{0},
                         T{0}, T{0}, T{1}, T{0}, T{0}, T{0}, T{0}, T{1}};
  EXPECT_EQ(mEn::value_ptr(m), &m[0].x);
}

TYPED_TEST(TypePtr, ValuePtrQuatPointsToX) {
  using T = TypeParam;

  mEn::qua<T> q{T{1}, T{0}, T{0}, T{0}};
  EXPECT_EQ(mEn::value_ptr(q), &q.x);
}

TYPED_TEST(TypePtr, ConstValuePtrVec2PointsToX) {
  using T = TypeParam;

  const mEn::vec<2, T> v{T{5}, T{6}};
  EXPECT_EQ(mEn::value_ptr(v), &v.x);
}

TYPED_TEST(TypePtr, ConstValuePtrQuatPointsToX) {
  using T = TypeParam;

  const mEn::qua<T> q{T{1}, T{0}, T{0}, T{0}};
  EXPECT_EQ(mEn::value_ptr(q), &q.x);
}

TYPED_TEST(TypePtr, MakeVec2RoundTrip) {
  using T = TypeParam;

  const mEn::vec<2, T> v{T{3}, T{7}};
  const auto v2 = mEn::make_vec2(mEn::value_ptr(v));
  EXPECT_VEC2_EQ(v2, EX(T, 3, 7));
}

TYPED_TEST(TypePtr, MakeVec3RoundTrip) {
  using T = TypeParam;

  const mEn::vec<3, T> v{T{1}, T{2}, T{3}};
  const auto v2 = mEn::make_vec3(mEn::value_ptr(v));
  EXPECT_VEC3_EQ(v2, EX(T, 1, 2, 3));
}

TYPED_TEST(TypePtr, MakeVec4RoundTrip) {
  using T = TypeParam;

  const mEn::vec<4, T> v{T{4}, T{5}, T{6}, T{7}};
  const auto v2 = mEn::make_vec4(mEn::value_ptr(v));
  EXPECT_VEC4_EQ(v2, EX(T, 4, 5, 6, 7));
}

TYPED_TEST(TypePtr, MakeMat3RoundTrip) {
  using T = TypeParam;

  const mEn::mat<3, T> m{T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}};
  const auto m2 = mEn::make_mat3(mEn::value_ptr(m));
  EXPECT_MAT3_EQ(m2, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TYPED_TEST(TypePtr, MakeMat4RoundTrip) {
  using T = TypeParam;

  const mEn::mat<4, T> m{T{1}, T{2},  T{3},  T{4},  T{5},  T{6},  T{7},  T{8},
                         T{9}, T{10}, T{11}, T{12}, T{13}, T{14}, T{15}, T{16}};
  const auto m2 = mEn::make_mat4(mEn::value_ptr(m));
  EXPECT_MAT4_EQ(m2, EX(T, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}

TYPED_TEST(TypePtr, MakeQuatRoundTrip) {
  using T = TypeParam;

  const mEn::qua<T> q{T{1}, T{2}, T{3}, T{4}};
  const auto q2 = mEn::make_quat(mEn::value_ptr(q));
  EXPECT_QUAT_EQ(q2, EX(T, q.x, q.y, q.z, q.w));
}

TYPED_TEST(TypePtr, MutableValuePtrVec3WritesBack) {
  using T = TypeParam;

  mEn::vec<3, T> v{T{0}, T{0}, T{0}};
  T* ptr = mEn::value_ptr(v);
  ptr[0] = T{10};
  ptr[1] = T{20};
  ptr[2] = T{30};
  EXPECT_VEC3_EQ(v, EX(T, 10, 20, 30));
}

TYPED_TEST(TypePtr, MutableValuePtrMat4WritesBack) {
  using T = TypeParam;

  mEn::mat<4, T> m{T{1}, T{0}, T{0}, T{0}, T{0}, T{1}, T{0}, T{0}, T{0}, T{0}, T{1}, T{0}, T{0}, T{0}, T{0}, T{1}};
  T* ptr = mEn::value_ptr(m);
  ptr[0] = T{9};
  EXPECT_EQ(m[0].x, T{9});
}

TYPED_TEST(TypePtr, MutableValuePtrQuatWritesBack) {
  using T = TypeParam;

  mEn::qua<T> q{T{1}, T{0}, T{0}, T{0}};
  T* ptr = mEn::value_ptr(q);
  ptr[0] = T{5};
  EXPECT_EQ(q.x, T{5});
}
