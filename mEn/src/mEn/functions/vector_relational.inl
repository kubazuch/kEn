#include <mEn/constants.hpp>
#include <mEn/fwd.hpp>

namespace mEn {
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, bool> lessThan(const vec<L, T>& x, const vec<L, T>& y) noexcept {
  vec<L, bool> ret(true);
  for (length_t i = 0; i < L; ++i) {
    ret[i] = x[i] < y[i];
  }
  return ret;
}

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, bool> lessThanEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept {
  vec<L, bool> ret(true);
  for (length_t i = 0; i < L; ++i) {
    ret[i] = x[i] <= y[i];
  }
  return ret;
}

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, bool> greaterThan(const vec<L, T>& x, const vec<L, T>& y) noexcept {
  vec<L, bool> ret(true);
  for (length_t i = 0; i < L; ++i) {
    ret[i] = x[i] > y[i];
  }
  return ret;
}

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, bool> greaterThanEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept {
  vec<L, bool> ret(true);
  for (length_t i = 0; i < L; ++i) {
    ret[i] = x[i] >= y[i];
  }
  return ret;
}

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, bool> equal(const vec<L, T>& x, const vec<L, T>& y) noexcept {
  vec<L, bool> ret(true);
  for (length_t i = 0; i < L; ++i) {
    if constexpr (Floating<T>) {
      const T eps = kEpsilon<T>;

      ret[i] = ((x[i] > y[i]) ? (x[i] - y[i]) : (y[i] - x[i])) <= eps;
    } else {
      ret[i] = x[i] == y[i];
    }
  }
  return ret;
}

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, bool> notEqual(const vec<L, T>& x, const vec<L, T>& y) noexcept {
  return not_(equal(x, y));
}

template <length_t L>
MEN_FORCE_INLINE constexpr bool any(const vec<L, bool>& v) noexcept {
  bool ret = false;
  for (length_t i = 0; i < L; ++i) {
    ret = ret || v[i];
  }
  return ret;
}

template <length_t L>
MEN_FORCE_INLINE constexpr bool all(const vec<L, bool>& v) noexcept {
  bool ret = true;
  for (length_t i = 0; i < L; ++i) {
    ret = ret && v[i];
  }
  return ret;
}

template <length_t L>
MEN_FORCE_INLINE constexpr vec<L, bool> not_(const vec<L, bool>& v) noexcept {  // NOLINT(readability-identifier-naming)
  vec<L, bool> ret(true);
  for (length_t i = 0; i < L; ++i) {
    ret[i] = !v[i];
  }
  return ret;
}

// NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}  // namespace mEn