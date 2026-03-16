#include <cmath>

#include <mEn/detail/vectorize.hpp>

namespace mEn {

template <VecOrScalar GenType>
MEN_FORCE_INLINE constexpr GenType radians(const GenType& degrees) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<&::mEn::radians<S>>(degrees);
  } else {
    static_assert(Floating<GenType>, "radians accepts only floating point types");
    return degrees * static_cast<GenType>(0.01745329251994329576923690768489);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE constexpr GenType degrees(const GenType& radians) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<&::mEn::degrees<S>>(radians);
  } else {
    static_assert(Floating<GenType>, "degrees accepts only floating point types");
    return radians * static_cast<GenType>(57.295779513082320876798154814105);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType sin(const GenType& angle) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::sin)>(angle);
  } else {
    return std::sin(angle);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType cos(const GenType& angle) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::cos)>(angle);
  } else {
    return std::cos(angle);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType tan(const GenType& angle) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::tan)>(angle);
  } else {
    return std::tan(angle);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType asin(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::asin)>(x);
  } else {
    return std::asin(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType acos(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::acos)>(x);
  } else {
    return std::acos(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType atan(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::atan)>(x);
  } else {
    return std::atan(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType atan(const GenType& y, const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::zip<static_cast<S (*)(S, S)>(std::atan2)>(y, x);
  } else {
    return std::atan2(y, x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType sinh(const GenType& angle) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::sinh)>(angle);
  } else {
    return std::sinh(angle);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType cosh(const GenType& angle) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::cosh)>(angle);
  } else {
    return std::cosh(angle);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType tanh(const GenType& angle) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::tanh)>(angle);
  } else {
    return std::tanh(angle);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType asinh(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::asinh)>(x);
  } else {
    return std::asinh(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType acosh(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::acosh)>(x);
  } else {
    return std::acosh(x);
  }
}

template <VecOrScalar GenType>
MEN_FORCE_INLINE GenType atanh(const GenType& x) noexcept {
  if constexpr (VecType<GenType>) {
    using S = scalar_of_t<GenType>;
    return detail::map<static_cast<S (*)(S)>(std::atanh)>(x);
  } else {
    return std::atanh(x);
  }
}

}  // namespace mEn