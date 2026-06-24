#include <gtest/gtest.h>

#include <type_traits>

#include <mEn/features/solve.hpp>
#include <mEn/fwd.hpp>

#include <glm/detail/qualifier.hpp>

#include "../util/mat_test_utils.hpp"
#include "../util/vec_test_utils.hpp"

namespace {

using men_test::GV;
using men_test::MM;
using men_test::MV;

template <mEn::length_t N, typename T>
void ExpectVecNear(const mEn::vec<N, T>& actual, const glm::vec<glm::length_t{N}, T>& expected) {
  const T tol = std::is_same_v<T, float> ? T{1e-4} : T{1e-10};
  for (mEn::length_t i = 0; i < N; ++i) {
    EXPECT_NEAR(actual[i], expected[i], tol) << "component " << i;
  }
}

template <mEn::length_t N, typename T>
auto Invertible() {
  if constexpr (N == 3) {
    return glm::mat<3, 3, T>(T{1}, T{2}, T{3}, T{0}, T{1}, T{4}, T{5}, T{6}, T{0});
  } else {
    return glm::mat<4, 4, T>(T{4}, T{0}, T{0}, T{0}, T{1}, T{3}, T{0}, T{0}, T{2}, T{1}, T{2}, T{0}, T{1}, T{0}, T{1},
                             T{1});
  }
}

template <mEn::length_t N, typename T>
void RunSolveRecoversKnownSolution() {
  const auto a = Invertible<N, T>();
  const auto x = GV<N, T>(T{1}, T{-2}, T{3}, T{4});
  const auto b = a * x;  // construct a consistent right-hand side

  mEn::vec<N, T> out{};
  const bool ok = mEn::solve(MM(a), MV(b), out);

  EXPECT_TRUE(ok);
  ExpectVecNear(out, x);
}

template <mEn::length_t N, typename T>
void RunSolveNeedsPivoting() {
  const auto a = [] {
    if constexpr (N == 3) {
      return glm::mat<3, 3, T>(T{0}, T{1}, T{2}, T{1}, T{1}, T{0}, T{2}, T{0}, T{1});
    } else {
      return glm::mat<4, 4, T>(T{0}, T{1}, T{0}, T{2}, T{1}, T{0}, T{1}, T{0}, T{0}, T{1}, T{2}, T{1}, T{3}, T{0}, T{0},
                               T{1});
    }
  }();
  const auto x = GV<N, T>(T{2}, T{-1}, T{4}, T{-3});
  const auto b = a * x;

  mEn::vec<N, T> out{};
  const bool ok = mEn::solve(MM(a), MV(b), out);

  EXPECT_TRUE(ok);
  ExpectVecNear(out, x);
}

template <mEn::length_t N, typename T>
void RunSolveSingularFails() {
  auto a = Invertible<N, T>();
  a[1]   = a[0] * T{2};  // make column 1 linearly dependent on column 0

  const auto b = GV<N, T>(T{1}, T{1}, T{1}, T{1});

  mEn::vec<N, T> out{};
  const bool ok = mEn::solve(MM(a), MV(b), out);

  EXPECT_FALSE(ok);
}

template <typename T>
struct Mat3Solve : ::testing::Test {};
template <typename T>
struct Mat4Solve : ::testing::Test {};

using TestedTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(Mat3Solve, TestedTypes);
TYPED_TEST_SUITE(Mat4Solve, TestedTypes);

}  // namespace

TYPED_TEST(Mat3Solve, RecoversKnownSolution) { RunSolveRecoversKnownSolution<3, TypeParam>(); }
TYPED_TEST(Mat3Solve, NeedsPivoting) { RunSolveNeedsPivoting<3, TypeParam>(); }
TYPED_TEST(Mat3Solve, SingularFails) { RunSolveSingularFails<3, TypeParam>(); }

TYPED_TEST(Mat4Solve, RecoversKnownSolution) { RunSolveRecoversKnownSolution<4, TypeParam>(); }
TYPED_TEST(Mat4Solve, NeedsPivoting) { RunSolveNeedsPivoting<4, TypeParam>(); }
TYPED_TEST(Mat4Solve, SingularFails) { RunSolveSingularFails<4, TypeParam>(); }
