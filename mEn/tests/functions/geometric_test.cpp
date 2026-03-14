#include <gtest/gtest.h>

#include <mEn/functions/geometric.hpp>

#include "../assert/scalar_eq.hpp"
#include "../util/vec_test_utils.hpp"

namespace {

using men_test::ExpectVecEq;
using men_test::GV;
using men_test::MV;
using men_test::V;

template <mEn::length_t L, typename T>
void RunDotTests() {
  const auto a    = GV<L, T>(T{1}, T{2}, T{3}, T{4});
  const auto b    = GV<L, T>(T{5}, T{-1}, T{0.5}, T{2});
  const auto zero = glm::vec<L, T>(T{0});

  EXPECT_SCALAR_EQ(mEn::dot(MV(a), MV(b)), glm::dot(a, b));
  EXPECT_SCALAR_EQ(mEn::dot(MV(b), MV(a)), glm::dot(b, a));
  EXPECT_SCALAR_EQ(mEn::dot(MV(a), MV(zero)), glm::dot(a, zero));
  EXPECT_SCALAR_EQ(mEn::dot(MV(a), MV(a)), glm::dot(a, a));
}

template <mEn::length_t L, typename T>
void RunLengthDistanceTests() {
  const auto x    = GV<L, T>(T{3}, T{4}, T{0}, T{0});
  const auto zero = glm::vec<L, T>(T{0});
  const auto z    = GV<L, T>(T{1}, T{-2}, T{2}, T{-1});

  EXPECT_SCALAR_EQ(mEn::length(MV(x)), glm::length(x));
  EXPECT_SCALAR_EQ(mEn::length(MV(zero)), glm::length(zero));
  EXPECT_SCALAR_EQ(mEn::distance(MV(x), MV(zero)), glm::distance(x, zero));
  EXPECT_SCALAR_EQ(mEn::distance(MV(x), MV(z)), glm::distance(x, z));
  EXPECT_SCALAR_EQ(mEn::distance(MV(z), MV(x)), glm::distance(z, x));
}

template <mEn::length_t L, typename T>
void RunNormalizeTests() {
  const auto x = GV<L, T>(T{3}, T{4}, T{12}, T{0});
  ExpectVecEq(mEn::normalize(MV(x)), glm::normalize(x));
}

template <mEn::length_t L, typename T>
void RunFaceforwardTests() {
  // NOLINTBEGIN(readability-identifier-naming)
  const auto N    = GV<L, T>(T{1}, T{0}, T{0}, T{0});
  const auto Nref = GV<L, T>(T{1}, T{0}, T{0}, T{0});
  const auto I1   = GV<L, T>(T{-1}, T{0}, T{0}, T{0});
  const auto I2   = GV<L, T>(T{1}, T{0}, T{0}, T{0});

  ExpectVecEq(mEn::faceforward(MV(N), MV(I1), MV(Nref)), glm::faceforward(N, I1, Nref));
  ExpectVecEq(mEn::faceforward(MV(N), MV(I2), MV(Nref)), glm::faceforward(N, I2, Nref));
  // NOLINTEND(readability-identifier-naming)
}

template <mEn::length_t L, typename T>
void RunReflectTests() {
  // NOLINTBEGIN(readability-identifier-naming)
  const auto N = GV<L, T>(T{0}, T{1}, T{0}, T{0});
  const auto I = GV<L, T>(T{1}, T{-1}, T{0.5}, T{2});

  ExpectVecEq(mEn::reflect(MV(I), MV(N)), glm::reflect(I, N));
  // NOLINTEND(readability-identifier-naming)
}

template <mEn::length_t L, typename T>
void RunRefractTests() {
  // NOLINTBEGIN(readability-identifier-naming)
  const auto N = GV<L, T>(T{0}, T{1}, T{0}, T{0});

  // Normal refraction case
  const auto I1 = GV<L, T>(T{0}, T{-1}, T{0}, T{0});
  ExpectVecEq(mEn::refract(MV(I1), MV(N), T{0.5}), glm::refract(I1, N, T{0.5}));

  // eta=2 with 45-degree incident angle triggers total internal reflection
  const auto I2 = glm::normalize(GV<L, T>(T{1}, T{-1}, T{0}, T{0}));
  ExpectVecEq(mEn::refract(MV(I2), MV(N), T{2}), glm::refract(I2, N, T{2}));
  // NOLINTEND(readability-identifier-naming)
}

template <typename T>
struct Vec2Geom : ::testing::Test {};
template <typename T>
struct Vec3Geom : ::testing::Test {};
template <typename T>
struct Vec4Geom : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Vec2Geom, TestedTypes);
TYPED_TEST_SUITE(Vec3Geom, TestedTypes);
TYPED_TEST_SUITE(Vec4Geom, TestedTypes);

}  // namespace

// ---- vec2 ----
TYPED_TEST(Vec2Geom, Dot) { RunDotTests<2, TypeParam>(); }
TYPED_TEST(Vec2Geom, LengthDistance) { RunLengthDistanceTests<2, TypeParam>(); }
TYPED_TEST(Vec2Geom, Normalize) { RunNormalizeTests<2, TypeParam>(); }
TYPED_TEST(Vec2Geom, Faceforward) { RunFaceforwardTests<2, TypeParam>(); }
TYPED_TEST(Vec2Geom, Reflect) { RunReflectTests<2, TypeParam>(); }
TYPED_TEST(Vec2Geom, Refract) { RunRefractTests<2, TypeParam>(); }

// ---- vec3 ----
TYPED_TEST(Vec3Geom, Dot) { RunDotTests<3, TypeParam>(); }
TYPED_TEST(Vec3Geom, Cross) {
  const auto x = GV<3, TypeParam>(TypeParam{1}, TypeParam{0}, TypeParam{0}, TypeParam{0});
  const auto y = GV<3, TypeParam>(TypeParam{0}, TypeParam{1}, TypeParam{0}, TypeParam{0});
  const auto a = GV<3, TypeParam>(TypeParam{1}, TypeParam{2}, TypeParam{3}, TypeParam{0});
  const auto b = GV<3, TypeParam>(TypeParam{4}, TypeParam{5}, TypeParam{6}, TypeParam{0});

  ExpectVecEq(mEn::cross(MV(x), MV(y)), glm::cross(x, y));
  ExpectVecEq(mEn::cross(MV(y), MV(x)), glm::cross(y, x));
  ExpectVecEq(mEn::cross(MV(x), MV(x)), glm::cross(x, x));
  ExpectVecEq(mEn::cross(MV(a), MV(b)), glm::cross(a, b));
}
TYPED_TEST(Vec3Geom, LengthDistance) { RunLengthDistanceTests<3, TypeParam>(); }
TYPED_TEST(Vec3Geom, Normalize) { RunNormalizeTests<3, TypeParam>(); }
TYPED_TEST(Vec3Geom, Faceforward) { RunFaceforwardTests<3, TypeParam>(); }
TYPED_TEST(Vec3Geom, Reflect) { RunReflectTests<3, TypeParam>(); }
TYPED_TEST(Vec3Geom, Refract) { RunRefractTests<3, TypeParam>(); }

// ---- vec4 ----
TYPED_TEST(Vec4Geom, Dot) { RunDotTests<4, TypeParam>(); }
TYPED_TEST(Vec4Geom, LengthDistance) { RunLengthDistanceTests<4, TypeParam>(); }
TYPED_TEST(Vec4Geom, Normalize) { RunNormalizeTests<4, TypeParam>(); }
TYPED_TEST(Vec4Geom, Faceforward) { RunFaceforwardTests<4, TypeParam>(); }
TYPED_TEST(Vec4Geom, Reflect) { RunReflectTests<4, TypeParam>(); }
TYPED_TEST(Vec4Geom, Refract) { RunRefractTests<4, TypeParam>(); }
