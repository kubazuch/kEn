#include <gtest/gtest.h>

#include <cstdint>
#include <type_traits>

#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>

#include <glm/detail/qualifier.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "assert/mat3_eq.hpp"
#include "assert/mat4_eq.hpp"
#include "util/test_utils.hpp"

namespace {

template <class T>
using Mat3 = mEn::mat<3, T>;
template <class T>
using Mat4 = mEn::mat<4, T>;

}  // namespace

TEST(MatConversions, Mat3FromMat4IsExplicitAndDropsLastRowAndColumn) {
  using Src = Mat4<std::int32_t>;
  using Dst = Mat3<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src m4{1, 2, 3, 100, 4, 5, 6, 200, 7, 8, 9, 300, 400, 500, 600, 700};

  const Dst m3{m4};
  EXPECT_MAT3_EQ(m3, EX(float, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F, 9.0F));
}

TEST(MatConversions, Mat3FromMat3IsExplicitAndCastsComponents) {
  using Src = Mat3<std::int32_t>;
  using Dst = Mat3<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src mi{1, 2, 3, 4, 5, 6, 7, 8, 9};

  const Dst mf{mi};
  EXPECT_MAT3_EQ(mf, EX(float, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F, 9.0F));
}

TEST(MatConversions, Mat4FromMat4IsExplicitAndCastsComponents) {
  using Src = Mat4<std::int32_t>;
  using Dst = Mat4<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src mi{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

  const Dst mf{mi};
  EXPECT_MAT4_EQ(mf, EX(float, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F, 9.0F, 10.0F, 11.0F, 12.0F, 13.0F, 14.0F,
                        15.0F, 16.0F));
}

TEST(MatConversions, ConversionsCastToDestinationScalarType) {
  const Mat4<float> f{1.9F, 2.1F,  3.5F,  4.9F,  5.2F,  6.8F,  7.1F,  8.0F,
                      9.9F, 10.0F, 11.4F, 12.6F, 13.0F, 14.2F, 15.9F, 16.1F};

  const Mat4<std::int32_t> i{f};
  EXPECT_MAT4_EQ(i, EX(std::int32_t, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}

TEST(MatConversions, Mat4FromMat3IsExplicitAndEmbedsTopLeftAndAddsIdentity) {
  using Src = Mat3<std::int32_t>;
  using Dst = Mat4<float>;

  static_assert(std::is_constructible_v<Dst, const Src&>);
  static_assert(!std::is_convertible_v<Src, Dst>);

  const Src m3{1, 2, 3, 4, 5, 6, 7, 8, 9};

  const Dst m4{m3};

  EXPECT_MAT4_EQ(
      m4, EX(float, 1.F, 2.0F, 3.0F, 0.0F, 4.0F, 5.0F, 6.0F, 0.0F, 7.0F, 8.0F, 9.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F));
}

TEST(MatGlmConversions, GlmMat3ToMenMat3IsImplicitAndPreservesColumns) {
  using Dst = Mat3<float>;
  using Src = glm::mat<3, 3, float, glm::defaultp>;
  using Col = glm::vec<3, float, glm::defaultp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gm{
      Col{1.0F, 2.0F, 3.0F},
      Col{4.0F, 5.0F, 6.0F},
      Col{7.0F, 8.0F, 9.0F},
  };

  const Dst m = gm;

  EXPECT_MAT3_EQ(m, EX(float, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F, 9.0F));
}

TEST(MatGlmConversions, GlmMat4ToMenMat4IsImplicitAndPreservesColumns) {
  using Dst = Mat4<float>;
  using Src = glm::mat<4, 4, float, glm::defaultp>;
  using Col = glm::vec<4, float, glm::defaultp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gm{
      Col{1.0F, 2.0F, 3.0F, 4.0F},
      Col{5.0F, 6.0F, 7.0F, 8.0F},
      Col{9.0F, 10.0F, 11.0F, 12.0F},
      Col{13.0F, 14.0F, 15.0F, 16.0F},
  };

  const Dst m = gm;

  EXPECT_MAT4_EQ(m, EX(float, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F, 9.0F, 10.0F, 11.0F, 12.0F, 13.0F, 14.0F,
                       15.0F, 16.0F));
}

TEST(MatGlmConversions, GlmMat3ToMenMat3IsImplicitAndCastsScalarType) {
  using Dst = Mat3<std::int32_t>;
  using Src = glm::mat<3, 3, double, glm::highp>;
  using Col = glm::vec<3, double, glm::highp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gm{
      Col{1.9, 2.1, 3.5},
      Col{4.9, 5.2, 6.8},
      Col{7.1, 8.0, 9.9},
  };

  const Dst m = gm;

  EXPECT_MAT3_EQ(m, EX(std::int32_t, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TEST(MatGlmConversions, GlmMat4ToMenMat4IsImplicitAndCastsScalarType) {
  using Dst = Mat4<std::int32_t>;
  using Src = glm::mat<4, 4, double, glm::defaultp>;
  using Col = glm::vec<4, double, glm::defaultp>;

  static_assert(std::is_convertible_v<Src, Dst>);

  const Src gm{
      Col{1.9, 2.1, 3.5, 4.9},
      Col{5.2, 6.8, 7.1, 8.0},
      Col{9.9, 10.0, 11.4, 12.6},
      Col{13.0, 14.2, 15.9, 16.1},
  };

  const Dst m = gm;

  EXPECT_MAT4_EQ(m, EX(std::int32_t, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
}

TEST(MatGlmConversions, MenMat3ToGlmMat3IsImplicitAndPreservesComponents) {
  using Src = Mat3<float>;
  using Dst = glm::mat<3, 3, float, glm::defaultp>;

  const Src m{1.0F, 2.0F, 3.0F,  //
              4.0F, 5.0F, 6.0F,  //
              7.0F, 8.0F, 9.0F};

  static_assert(std::is_convertible_v<Src, Dst>);

  const Dst gm = m;
  EXPECT_MAT3_EQ(m, gm);
}

TEST(MatGlmConversions, MenMat4ToGlmMat4IsImplicitAndPreservesComponents) {
  using Src = Mat4<float>;
  using Dst = glm::mat<4, 4, float, glm::defaultp>;

  const Src m{1.0F,  2.0F,  3.0F,  4.0F,   //
              5.0F,  6.0F,  7.0F,  8.0F,   //
              9.0F,  10.0F, 11.0F, 12.0F,  //
              13.0F, 14.0F, 15.0F, 16.0F};

  static_assert(std::is_convertible_v<Src, Dst>);

  const Dst gm = m;
  EXPECT_MAT4_EQ(m, gm);
}

TEST(MatGlmConversions, MenMat3ToGlmMat3WithNonDefaultQualifierWorks) {
  using Src = Mat3<float>;
  using Dst = glm::mat<3, 3, float, glm::highp>;

  const Src m{1.0F, 2.0F, 3.0F,  //
              4.0F, 5.0F, 6.0F,  //
              7.0F, 8.0F, 9.0F};

  static_assert(std::is_convertible_v<Src, Dst>);

  const Dst gm = m;
  EXPECT_MAT3_EQ(m, gm);
}

TEST(MatGlmConversions, MenMat4ToGlmMat4WithNonDefaultQualifierWorks) {
  using Src = Mat4<float>;
  using Dst = glm::mat<4, 4, float, glm::highp>;

  const Src m{1.0F,  2.0F,  3.0F,  4.0F,   //
              5.0F,  6.0F,  7.0F,  8.0F,   //
              9.0F,  10.0F, 11.0F, 12.0F,  //
              13.0F, 14.0F, 15.0F, 16.0F};

  static_assert(std::is_convertible_v<Src, Dst>);

  const Dst gm = m;
  EXPECT_MAT4_EQ(m, gm);
}
