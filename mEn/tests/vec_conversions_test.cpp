#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

#include <glm/detail/qualifier.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "assert/vec2_eq.hpp"
#include "assert/vec3_eq.hpp"
#include "assert/vec4_eq.hpp"
#include "util/test_utils.hpp"

namespace {

template <class T>
using Vec2 = mEn::vec<2, T>;
template <class T>
using Vec3 = mEn::vec<3, T>;
template <class T>
using Vec4 = mEn::vec<4, T>;

}  // namespace

TEST(VecConversions, Vec2FromVec3IsExplicitAndDropsZ) {
  using Src = Vec3<std::int32_t>;
  using Dst = Vec2<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src v3{1, 2, 999};
  const Dst v2{v3};
  EXPECT_VEC2_EQ(v2, EX(float, 1.0F, 2.0F));
}

TEST(VecConversions, Vec2FromVec4IsExplicitAndDropsZW) {
  using Src = Vec4<std::size_t>;
  using Dst = Vec2<std::int32_t>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src v4{10, 20, 30, 40};
  const Dst v2{v4};
  EXPECT_VEC2_EQ(v2, EX(std::int32_t, 10, 20));
}

TEST(VecConversions, Vec3FromVec4IsExplicitAndDropsW) {
  using Src = Vec4<std::int32_t>;
  using Dst = Vec3<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src v4{1, 2, 3, 777};
  const Dst v3{v4};
  EXPECT_VEC3_EQ(v3, EX(float, 1.0F, 2.0F, 3.0F));
}

TEST(VecConversions, Vec2FromVec2IsExplicitAndCastsComponents) {
  using Src = Vec2<std::int32_t>;
  using Dst = Vec2<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src v2i{1, 2};
  const Dst v2f{v2i};
  EXPECT_VEC2_EQ(v2f, EX(float, 1.0F, 2.0F));
}

TEST(VecConversions, Vec3FromVec3IsExplicitAndCastsComponents) {
  using Src = Vec3<std::int32_t>;
  using Dst = Vec3<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src v3i{1, 2, 3};
  const Dst v3f{v3i};
  EXPECT_VEC3_EQ(v3f, EX(float, 1.0F, 2.0F, 3.0F));
}

TEST(VecConversions, Vec4FromVec4IsExplicitAndCastsComponents) {
  using Src = Vec4<std::int32_t>;
  using Dst = Vec4<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src v4i{1, 2, 3, 4};
  const Dst v4f{v4i};
  EXPECT_VEC4_EQ(v4f, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(VecConversions, Vec3FromVec2AndScalar) {
  const Vec2<std::int32_t> xy{1, 2};
  const Vec3<float> v{xy, 3.5F};
  EXPECT_VEC3_EQ(v, EX(float, 1.0F, 2.0F, 3.5F));
}

TEST(VecConversions, Vec3FromScalarAndVec2) {
  const Vec2<std::int32_t> yz{2, 3};
  const Vec3<float> v{0.5F, yz};
  EXPECT_VEC3_EQ(v, EX(float, 0.5F, 2.0F, 3.0F));
}

TEST(VecConversions, Vec4FromVec2AndTwoScalars) {
  const Vec2<std::int32_t> xy{1, 2};
  const Vec4<float> v{xy, 3.0F, 4.0F};
  EXPECT_VEC4_EQ(v, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(VecConversions, Vec4FromScalarVec2Scalar) {
  const Vec2<std::int32_t> yz{2, 3};
  const Vec4<float> v{1.0F, yz, 4.0F};
  EXPECT_VEC4_EQ(v, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(VecConversions, Vec4FromTwoScalarsAndVec2) {
  const Vec2<std::int32_t> zw{3, 4};
  const Vec4<float> v{1.0F, 2.0F, zw};
  EXPECT_VEC4_EQ(v, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(VecConversions, Vec4FromScalarAndVec3) {
  const Vec3<std::int32_t> yzw{2, 3, 4};
  const Vec4<float> v{1.0F, yzw};
  EXPECT_VEC4_EQ(v, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(VecConversions, Vec4FromVec3AndScalar) {
  const Vec3<std::int32_t> xyz{1, 2, 3};
  const Vec4<float> v{xyz, 4.0F};
  EXPECT_VEC4_EQ(v, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(VecConversions, Vec4FromTwoVec2) {
  const Vec2<std::int32_t> xy{1, 2};
  const Vec2<std::int32_t> zw{3, 4};
  const Vec4<float> v{xy, zw};
  EXPECT_VEC4_EQ(v, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(VecConversions, ConversionsCastToDestinationScalarType) {
  const Vec4<std::int32_t> i{1, 2, 3, 4};
  const Vec2<float> f2{i};
  EXPECT_VEC2_EQ(f2, EX(float, 1.0F, 2.0F));

  const Vec4<float> f4{1.9F, 2.1F, 3.5F, 4.9F};
  const Vec4<std::int32_t> i4{f4};
  EXPECT_VEC4_EQ(i4, EX(std::int32_t, 1, 2, 3, 4));
}

TEST(VecGlmConversions, GlmVec2ToMenVec2IsImplicitAndPreservesComponents) {
  using Dst = Vec2<float>;
  using Src = glm::vec<2, int, glm::highp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gv{1, 2};
  const Dst v = gv;

  EXPECT_VEC2_EQ(v, EX(float, 1.0F, 2.0F));
}

TEST(VecGlmConversions, GlmVec3ToMenVec3IsImplicitAndPreservesComponents) {
  using Dst = Vec3<float>;
  using Src = glm::vec<3, float, glm::defaultp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gv{1.0F, 2.0F, 3.0F};
  const Dst v = gv;

  EXPECT_VEC3_EQ(v, EX(float, 1.0F, 2.0F, 3.0F));
}

TEST(VecGlmConversions, GlmVec4ToMenVec4IsImplicitAndPreservesComponents) {
  using Dst = Vec4<float>;
  using Src = glm::vec<4, float, glm::defaultp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gv{1.0F, 2.0F, 3.0F, 4.0F};
  const Dst v = gv;

  EXPECT_VEC4_EQ(v, EX(float, 1.0F, 2.0F, 3.0F, 4.0F));
}

TEST(VecGlmConversions, GlmVec2ToMenVec2CastsScalarType) {
  using Dst = Vec2<std::int32_t>;
  using Src = glm::vec<2, double, glm::highp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gv{1.9, 2.1};
  const Dst v = gv;

  EXPECT_VEC2_EQ(v, EX(std::int32_t, 1, 2));
}

TEST(VecGlmConversions, GlmVec3ToMenVec3CastsScalarType) {
  using Dst = Vec3<std::int32_t>;
  using Src = glm::vec<3, double, glm::highp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gv{1.9, 2.1, 3.5};
  const Dst v = gv;

  EXPECT_VEC3_EQ(v, EX(std::int32_t, 1, 2, 3));
}

TEST(VecGlmConversions, GlmVec4ToMenVec4CastsScalarType) {
  using Dst = Vec4<std::int32_t>;
  using Src = glm::vec<4, double, glm::highp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gv{1.9, 2.1, 3.5, 4.9};
  const Dst v = gv;

  EXPECT_VEC4_EQ(v, EX(std::int32_t, 1, 2, 3, 4));
}

TEST(VecGlmConversions, MenVec2ToGlmVec2IsImplicitAndPreservesComponents) {
  using Src = Vec2<float>;
  using Dst = glm::vec<2, float, glm::defaultp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src v{1.0F, 2.0F};
  const Dst gv = v;

  EXPECT_FLOAT_EQ(gv.x, 1.0F);
  EXPECT_FLOAT_EQ(gv.y, 2.0F);
}

TEST(VecGlmConversions, MenVec3ToGlmVec3IsImplicitAndPreservesComponents) {
  using Src = Vec3<float>;
  using Dst = glm::vec<3, float, glm::defaultp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src v{1.0F, 2.0F, 3.0F};
  const Dst gv = v;

  EXPECT_FLOAT_EQ(gv.x, 1.0F);
  EXPECT_FLOAT_EQ(gv.y, 2.0F);
  EXPECT_FLOAT_EQ(gv.z, 3.0F);
}

TEST(VecGlmConversions, MenVec4ToGlmVec4IsImplicitAndPreservesComponents) {
  using Src = Vec4<float>;
  using Dst = glm::vec<4, float, glm::defaultp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src v{1.0F, 2.0F, 3.0F, 4.0F};
  const Dst gv = v;

  EXPECT_FLOAT_EQ(gv.x, 1.0F);
  EXPECT_FLOAT_EQ(gv.y, 2.0F);
  EXPECT_FLOAT_EQ(gv.z, 3.0F);
  EXPECT_FLOAT_EQ(gv.w, 4.0F);
}

TEST(VecGlmConversions, MenVec2ToGlmVec2WithNonDefaultQualifierWorks) {
  using Src = Vec2<double>;
  using Dst = glm::vec<2, double, glm::highp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src v{1.9, 2.1};
  const Dst gv = v;

  EXPECT_FLOAT_EQ(gv.x, 1.9);
  EXPECT_FLOAT_EQ(gv.y, 2.1);
}

TEST(VecGlmConversions, MenVec3ToGlmVec3WithNonDefaultQualifierWorks) {
  using Src = Vec3<double>;
  using Dst = glm::vec<3, double, glm::highp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src v{1.9, 2.1, 3.5};
  const Dst gv = v;

  EXPECT_FLOAT_EQ(gv.x, 1.9);
  EXPECT_FLOAT_EQ(gv.y, 2.1);
  EXPECT_FLOAT_EQ(gv.z, 3.5);
}

TEST(VecGlmConversions, MenVec4ToGlmVec4WithNonDefaultQualifierWorks) {
  using Src = Vec4<double>;
  using Dst = glm::vec<4, double, glm::highp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src v{1.9, 2.1, 3.5, 4.9};
  const Dst gv = v;

  EXPECT_FLOAT_EQ(gv.x, 1.9);
  EXPECT_FLOAT_EQ(gv.y, 2.1);
  EXPECT_FLOAT_EQ(gv.z, 3.5);
  EXPECT_FLOAT_EQ(gv.w, 4.9);
}