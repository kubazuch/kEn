#pragma once
#ifndef MEN_GLM
#include <cassert>
#include <cmath>

namespace mEn {

template <int L, typename T>
struct vec;

template <int N, typename T>
struct mat;

static constexpr float kEpsilon = 1e-6F;

}  // namespace mEn
#endif