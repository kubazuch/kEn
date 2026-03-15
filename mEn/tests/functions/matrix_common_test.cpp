#include <gtest/gtest.h>

#include <cstdint>
#include <type_traits>

#include <mEn/functions/matrix_common.hpp>
#include <mEn/fwd.hpp>

#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_integer.hpp>

#include "../assert/scalar_eq.hpp"
#include "../util/mat_test_utils.hpp"
#include "../util/vec_test_utils.hpp"

namespace {
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)

using men_test::ExpectMatEq;
using men_test::GV;
using men_test::M;
using men_test::MM;
using men_test::MV;

template <mEn::length_t N, typename T>
void RunMatrixCompMult() {
  const auto ga = [] {
    if constexpr (N == 3) {
      return glm::mat<3, 3, T>(T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9});
    } else {
      return glm::mat<4, 4, T>(T{1}, T{2}, T{3}, T{4}, T{5}, T{6}, T{7}, T{8}, T{9}, T{10}, T{11}, T{12}, T{13}, T{14},
                               T{15}, T{16});
    }
  }();

  const auto gb = [] {
    if constexpr (N == 3) {
      return glm::mat<3, 3, T>(T{2}, T{0}, T{-1}, T{3}, T{1}, T{2}, T{-2}, T{4}, T{1});
    } else {
      return glm::mat<4, 4, T>(T{2}, T{0}, T{-1}, T{3}, T{1}, T{2}, T{0}, T{-2}, T{4}, T{1}, T{2}, T{0}, T{-3}, T{2},
                               T{1}, T{2});
    }
  }();

  ExpectMatEq(mEn::matrixCompMult(MM(ga), MM(gb)), glm::matrixCompMult(ga, gb));
}

template <mEn::length_t N, typename T>
void RunOuterProduct() {
  const auto c = GV<N, T>(T{1}, T{-2}, T{3}, T{4});
  const auto r = GV<N, T>(T{2}, static_cast<T>(0.5), T{-1}, T{3});

  ExpectMatEq(mEn::outerProduct(MV(c), MV(r)), glm::outerProduct(c, r));
}

template <mEn::length_t N, typename T>
void RunTranspose() {
  const auto ga = [] {
    if constexpr (N == 3) {
      return glm::mat<3, 3, T>(T{1}, T{2}, T{3}, T{0}, T{1}, T{4}, T{5}, T{6}, T{0});
    } else {
      return glm::mat<4, 4, T>(T{1}, T{2}, T{3}, T{4}, T{0}, T{1}, T{4}, T{0}, T{5}, T{6}, T{0}, T{0}, T{7}, T{8}, T{9},
                               T{1});
    }
  }();

  ExpectMatEq(mEn::transpose(MM(ga)), glm::transpose(ga));
  ExpectMatEq(mEn::transpose(mEn::transpose(MM(ga))), ga);
}

template <mEn::length_t N, typename T>
void RunDeterminant() {
  // NOLINTBEGIN(readability-identifier-naming)
  const auto I = glm::mat<N, N, T>(T{1});
  EXPECT_SCALAR_EQ(mEn::determinant(MM(I)), glm::determinant(I));

  const auto diag = GV<N, T>(T{2}, T{3}, T{4}, T{5});
  auto D          = glm::mat<N, N, T>(T{0});
  for (mEn::length_t i = 0; i < N; ++i) {
    D[i][i] = diag[i];
  }
  EXPECT_SCALAR_EQ(mEn::determinant(MM(D)), glm::determinant(D));

  const auto A = [] {
    if constexpr (N == 3) {
      return glm::mat<3, 3, T>(T{1}, T{2}, T{3}, T{0}, T{1}, T{4}, T{5}, T{6}, T{0});
    } else {
      return glm::mat<4, 4, T>(T{1}, T{2}, T{3}, T{4}, T{0}, T{1}, T{4}, T{0}, T{5}, T{6}, T{0}, T{0}, T{7}, T{8}, T{9},
                               T{1});
    }
  }();
  EXPECT_SCALAR_EQ(mEn::determinant(MM(A)), glm::determinant(A));
  EXPECT_SCALAR_EQ(mEn::determinant(MM(A)), mEn::determinant(mEn::transpose(MM(A))));
  // NOLINTEND(readability-identifier-naming)
}

template <mEn::length_t N, typename T>
void RunInverse() {
  // NOLINTBEGIN(readability-identifier-naming)
  if constexpr (!std::is_floating_point_v<T>) {
    GTEST_SKIP() << "inverse() is only validated for floating-point types";
  } else {
    const auto diag = GV<N, T>(T{2}, T{3}, T{4}, T{5});
    auto D          = glm::mat<N, N, T>(T{0});
    for (mEn::length_t i = 0; i < N; ++i) {
      D[i][i] = diag[i];
    }

    ExpectMatEq(mEn::inverse(MM(D)), glm::inverse(D));
    ExpectMatEq(MM(D) * mEn::inverse(MM(D)), glm::mat<N, N, T>(T{1}));
    ExpectMatEq(mEn::inverse(MM(D)) * MM(D), glm::mat<N, N, T>(T{1}));

    const auto A = [] {
      if constexpr (N == 3) {
        return glm::mat<3, 3, T>(T{1}, T{2}, T{3}, T{0}, T{1}, T{4}, T{5}, T{6}, T{0});
      } else {
        return glm::mat<4, 4, T>(T{4}, T{0}, T{0}, T{0}, T{1}, T{3}, T{0}, T{0}, T{2}, T{1}, T{2}, T{0}, T{1}, T{0},
                                 T{1}, T{1});
      }
    }();

    ExpectMatEq(mEn::inverse(MM(A)), glm::inverse(A));
    ExpectMatEq(MM(A) * mEn::inverse(MM(A)), glm::mat<N, N, T>(T{1}));
  }
  // NOLINTEND(readability-identifier-naming)
}

template <typename T>
struct Mat3Common : ::testing::Test {};
template <typename T>
struct Mat4Common : ::testing::Test {};

using TestedTypes = ::testing::Types<std::int32_t, float, double>;
TYPED_TEST_SUITE(Mat3Common, TestedTypes);
TYPED_TEST_SUITE(Mat4Common, TestedTypes);

// NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}  // namespace

TYPED_TEST(Mat3Common, MatrixCompMult) { RunMatrixCompMult<3, TypeParam>(); }
TYPED_TEST(Mat3Common, OuterProduct) { RunOuterProduct<3, TypeParam>(); }
TYPED_TEST(Mat3Common, Transpose) { RunTranspose<3, TypeParam>(); }
TYPED_TEST(Mat3Common, Determinant) { RunDeterminant<3, TypeParam>(); }
TYPED_TEST(Mat3Common, Inverse) { RunInverse<3, TypeParam>(); }

TYPED_TEST(Mat4Common, MatrixCompMult) { RunMatrixCompMult<4, TypeParam>(); }
TYPED_TEST(Mat4Common, OuterProduct) { RunOuterProduct<4, TypeParam>(); }
TYPED_TEST(Mat4Common, Transpose) { RunTranspose<4, TypeParam>(); }
TYPED_TEST(Mat4Common, Determinant) { RunDeterminant<4, TypeParam>(); }
TYPED_TEST(Mat4Common, Inverse) { RunInverse<4, TypeParam>(); }
