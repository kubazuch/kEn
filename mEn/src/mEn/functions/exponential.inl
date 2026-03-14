#include <cmath>

#include <mEn/detail/vectorize.hpp>

namespace mEn {

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType pow(const GenType& base, const GenType& exponent) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::zip<static_cast<S (*)(S, S)>(std::pow)>(base, exponent);
  } else {
    return std::pow(base, exponent);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType exp(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::exp)>(x);
  } else {
    return std::exp(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType log(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::log)>(x);
  } else {
    return std::log(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType exp2(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::exp2)>(x);
  } else {
    return std::exp2(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType log2(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::log2)>(x);
  } else {
    return std::log2(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType log10(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::log10)>(x);
  } else {
    return std::log10(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType sqrt(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::sqrt)>(x);
  } else {
    return std::sqrt(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType inversesqrt(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<&::mEn::inversesqrt<S>>(x);
  } else {
    return GenType{1} / sqrt(x);
  }
}

}  // namespace mEn
