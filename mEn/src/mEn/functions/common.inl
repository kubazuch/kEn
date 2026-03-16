#include <cmath>

#include <mEn/detail/vectorize.hpp>

namespace mEn {

template <VecOrScalar GenType>
MEN_FORCE_INLINE constexpr GenType abs(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::abs)>(x);
  } else {
    return std::abs(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE constexpr GenType sign(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<&::mEn::sign<S>>(x);
  } else {
    return (GenType{0} < x) - (x < GenType{0});
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType floor(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::floor)>(x);
  } else {
    return std::floor(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType trunc(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::trunc)>(x);
  } else {
    return std::trunc(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType round(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::round)>(x);
  } else {
    return std::round(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType roundEven(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<&::mEn::roundEven<S>>(x);
  } else {
    GenType i = floor(x);
    GenType f = x - i;

    if (f < GenType{0.5}) {
      return i;
    }
    if (f > GenType{0.5}) {
      return i + GenType{1};
    }

    return (mod(i, GenType{2}) == GenType{0} ? i : (i + GenType{1}));
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType ceil(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::ceil)>(x);
  } else {
    return std::ceil(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType fract(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<&::mEn::fract<S>>(x);
  } else {
    return x - floor(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType mod(const GenType& x, const GenType& y) noexcept {
  return x - (y * floor(x / y));
}

template <length_t L, typename T>
MEN_FORCE_INLINE vec<L, T> mod(const vec<L, T>& x, T y) noexcept {
  return x - (y * floor(x / y));
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE constexpr GenType min(const GenType& x, const GenType& y) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    using F = const S& (*)(const S&, const S&);
    return detail::zip<static_cast<F>(std::min)>(x, y);
  } else {
    return std::min(x, y);
  }
}

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, T> min(const vec<L, T>& x, T y) noexcept {
  using F = const T& (*)(const T&, const T&);
  return detail::zip<static_cast<F>(std::min)>(x, y);
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE constexpr GenType max(const GenType& x, const GenType& y) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    using F = const S& (*)(const S&, const S&);
    return detail::zip<static_cast<F>(std::max)>(x, y);
  } else {
    return std::max(x, y);
  }
}

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, T> max(const vec<L, T>& x, T y) noexcept {
  using F = const T& (*)(const T&, const T&);
  return detail::zip<static_cast<F>(std::max)>(x, y);
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE constexpr GenType clamp(const GenType& x, const GenType& from, const GenType& to) noexcept {
  return min(max(x, from), to);
}

template <length_t L, typename T>
MEN_FORCE_INLINE constexpr vec<L, T> clamp(const vec<L, T>& x, T from, T to) noexcept {
  return min(max(x, from), to);
}

template <VecOrScalar GenType, typename U>
  requires std::same_as<U, bool> || std::same_as<U, scalar_of_t<GenType>>
MEN_FORCE_INLINE constexpr GenType mix(const GenType& x, const GenType& y, U t) noexcept {
  if constexpr (std::same_as<U, bool>) {
    return t ? y : x;
  } else {
    return (x * (U{1} - t)) + (y * t);
  }
}

template <length_t L, typename T, typename U>
  requires std::same_as<U, bool> || std::same_as<U, T>
MEN_FORCE_INLINE constexpr vec<L, T> mix(const vec<L, T>& x, const vec<L, T>& y, const vec<L, U>& t) noexcept {
  if constexpr (std::same_as<U, bool>) {
    return detail::zip<&::mEn::mix<T, bool>>(x, y, t);
  } else {
    return (x * (vec<L, T>{T{1}} - t)) + (y * t);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType step(const GenType& edge, const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::zip<&::mEn::step<S>>(edge, x);
  } else {
    return x < edge ? GenType{0} : GenType{1};
  }
}

template <length_t L, typename T>
MEN_FORCE_INLINE vec<L, T> step(T edge, const vec<L, T>& x) noexcept {
  return detail::zip<&::mEn::step<T>>(edge, x);
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType smoothstep(const GenType& edge1, const GenType& edge2, const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S           = scalar_of_t<GenType>;
    const GenType tmp = clamp((x - edge1) / (edge2 - edge1), S{0}, S{1});
    return tmp * tmp * (S{3} - (S{2} * tmp));
  } else {
    const GenType tmp = clamp((x - edge1) / (edge2 - edge1), GenType{0}, GenType{1});
    return tmp * tmp * (GenType{3} - (GenType{2} * tmp));
  }
}

template <length_t L, typename T>
MEN_FORCE_INLINE vec<L, T> smoothstep(T edge1, T edge2, const vec<L, T>& x) noexcept {
  const vec<L, T> tmp(clamp((x - edge1) / (edge2 - edge1), T{0}, T{1}));
  return tmp * tmp * (T{3} - (T{2} * tmp));
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE bool_result_t<GenType> isnan(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<bool (*)(S)>(std::isnan)>(x);
  } else {
    return std::isnan(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE bool_result_t<GenType> isinf(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<bool (*)(S)>(std::isinf)>(x);
  } else {
    return std::isinf(x);
  }
}

}  // namespace mEn