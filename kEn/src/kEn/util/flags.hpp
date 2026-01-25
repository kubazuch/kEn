#pragma once

#include <bit>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

/** @file
 *  @ingroup ken
 */

namespace kEn::util {  // NOLINT

/**
 * @brief Opt-in trait enabling the enum flag operators in this header.
 *
 * Specialize this trait for your enum type and inherit from @c std::true_type to enable
 * the free operator overloads (|, &, ^, ~) that return @ref Flags.
 *
 * @tparam E Enum type.
 * @see KEN_ENABLE_FLAGS
 */
template <typename E>
struct enable_flags : std::false_type {};  // NOLINT(readability-identifier-naming)

/**
 * @brief Convenience variable template for @ref enable_flags.
 * @tparam E Enum type.
 */
template <typename E>
inline constexpr bool enable_flags_v = enable_flags<E>::value;  // NOLINT(readability-identifier-naming)

/**
 * @brief Concept satisfied by enum types used as flags.
 * @tparam E Type to test.
 */
template <typename E>
concept EnumFlag = std::is_enum_v<E>;

/**
 * @brief Type-safe wrapper around a bitmask of enum flags.
 *
 * @tparam E Enum type representing individual flag values.
 *
 * @note This class stores the bitmask in an unsigned form of the enum's underlying type.
 *       It does not validate that enum values are powers of two; iteration helpers assume
 *       one flag per set bit.
 */
template <EnumFlag E>
class Flags final {
 public:
  /** @brief The enum type represented by this flag set. */
  using enum_type = E;
  /** @brief Unsigned underlying integer type used to store the bits. */
  using underlying_type = std::make_unsigned_t<std::underlying_type_t<E>>;

 private:
  /** @brief Raw stored bitmask. */
  underlying_type bits_{0};

  /**
   * @brief Convert an enum value to its underlying bit representation.
   * @param e Enum value.
   * @return Underlying bit pattern for @p e.
   */
  static constexpr underlying_type to_bits(E e) noexcept { return static_cast<underlying_type>(e); }

 public:
  /** @brief Construct an empty flag set (no bits set). */
  constexpr Flags() noexcept = default;

  /**
   * @brief Construct from a single enum flag value.
   * @param e Flag value to set.
   */
  constexpr Flags(E e) noexcept : bits_(to_bits(e)) {}  // NOLINT(google-explicit-constructor)

  /**
   * @brief Construct from a raw bitmask value.
   * @param bits Raw bitmask.
   */
  constexpr explicit Flags(underlying_type bits) noexcept : bits_(bits) {}

  /**
   * @brief Construct from a list of enum flag values.
   * @param init List of flags to set.
   */
  constexpr Flags(std::initializer_list<E> init) noexcept {
    for (E e : init) {
      bits_ |= to_bits(e);
    }
  }

  /**
   * @brief Get the raw bitmask value.
   * @return Raw bitmask.
   */
  [[nodiscard]] constexpr underlying_type value() const noexcept { return bits_; }

  /** @brief @return True if any bit is set. */
  [[nodiscard]] constexpr bool any() const noexcept { return bits_ != 0; }
  /** @brief @return True if no bits are set. */
  [[nodiscard]] constexpr bool none() const noexcept { return bits_ == 0; }
  /**
   * @brief Explicit boolean conversion.
   * @return Same as @ref any().
   */
  [[nodiscard]] constexpr explicit operator bool() const noexcept { return any(); }

  /**
   * @brief Test whether a flag bit is set.
   * @param e Flag to test.
   * @return True if @p e is set.
   */
  [[nodiscard]] constexpr bool test(E e) const noexcept { return (bits_ & to_bits(e)) != 0; }

  /**
   * @brief Set the given flag bit.
   * @param e Flag to set.
   * @return *this.
   */
  constexpr Flags& set(E e) noexcept {
    bits_ |= to_bits(e);
    return *this;
  }

  /**
   * @brief Clear the given flag bit.
   * @param e Flag to clear.
   * @return *this.
   */
  constexpr Flags& reset(E e) noexcept {
    bits_ &= static_cast<underlying_type>(~to_bits(e));
    return *this;
  }

  /**
   * @brief Toggle the given flag bit.
   * @param e Flag to toggle.
   * @return *this.
   */
  constexpr Flags& toggle(E e) noexcept {
    bits_ ^= to_bits(e);
    return *this;
  }

  /**
   * @brief Clear all bits.
   * @return *this.
   */
  constexpr Flags& clear() noexcept {
    bits_ = 0;
    return *this;
  }

  /**
   * @brief Count the number of set bits.
   * @return Number of bits set in this mask.
   */
  [[nodiscard]] constexpr std::size_t count() const noexcept { return std::popcount(bits_); }

  /**
   * @brief Check whether all bits from @p rhs are set in this mask.
   * @param rhs Mask of bits to test.
   * @return True if this mask contains all bits in @p rhs.
   */
  [[nodiscard]] constexpr bool contains_all(Flags rhs) const noexcept { return (bits_ & rhs.bits_) == rhs.bits_; }

  /**
   * @brief Check whether any bit from @p rhs is set in this mask.
   * @param rhs Mask of bits to test.
   * @return True if this mask contains at least one bit from @p rhs.
   */
  [[nodiscard]] constexpr bool contains_any(Flags rhs) const noexcept { return (bits_ & rhs.bits_) != 0; }

  /**
   * @brief Invoke a callback for every set bit in the mask.
   *
   * For each set bit position, the callback receives an @c E value whose underlying representation
   * is exactly that single bit (i.e., @c 1U<<idx).
   *
   * @tparam Fn Callable type.
   * @param fn Callable to invoke for each set bit.
   *
   * @note Intended for enums where each flag is a single-bit value.
   */
  template <typename Fn>
  constexpr void for_each_set(Fn&& fn) const noexcept(noexcept(std::forward<Fn>(fn)(E{}))) {
    underlying_type v = bits_;
    while (v != 0) {
      const int idx  = std::countr_zero(v);
      const auto bit = static_cast<underlying_type>(static_cast<underlying_type>(1U) << idx);
      std::forward<Fn>(fn)(static_cast<E>(bit));
      v &= static_cast<underlying_type>(v - 1);
    }
  }

  /**
   * @brief In-place bitwise OR with another mask.
   * @param rhs Mask to OR in.
   * @return *this.
   */
  constexpr Flags& operator|=(Flags rhs) noexcept {
    bits_ |= rhs.bits_;
    return *this;
  }

  /**
   * @brief In-place bitwise AND with another mask.
   * @param rhs Mask to AND in.
   * @return *this.
   */
  constexpr Flags& operator&=(Flags rhs) noexcept {
    bits_ &= rhs.bits_;
    return *this;
  }

  /**
   * @brief In-place bitwise XOR with another mask.
   * @param rhs Mask to XOR in.
   * @return *this.
   */
  constexpr Flags& operator^=(Flags rhs) noexcept {
    bits_ ^= rhs.bits_;
    return *this;
  }

  /**
   * @brief In-place bitwise OR with a single enum flag.
   * @param e Flag to OR in.
   * @return *this.
   */
  constexpr Flags& operator|=(E e) noexcept { return (*this |= Flags{e}); }
  /**
   * @brief In-place bitwise AND with a single enum flag.
   * @param e Flag to AND in.
   * @return *this.
   */
  constexpr Flags& operator&=(E e) noexcept { return (*this &= Flags{e}); }
  /**
   * @brief In-place bitwise XOR with a single enum flag.
   * @param e Flag to XOR in.
   * @return *this.
   */
  constexpr Flags& operator^=(E e) noexcept { return (*this ^= Flags{e}); }

  /**
   * @brief Bitwise OR between two flag masks.
   * @param lhs Left-hand mask.
   * @param rhs Right-hand mask.
   * @return Combined mask.
   */
  friend constexpr Flags operator|(Flags lhs, Flags rhs) noexcept {
    lhs |= rhs;
    return lhs;
  }

  /**
   * @brief Bitwise AND between two flag masks.
   * @param lhs Left-hand mask.
   * @param rhs Right-hand mask.
   * @return Intersected mask.
   */
  friend constexpr Flags operator&(Flags lhs, Flags rhs) noexcept {
    lhs &= rhs;
    return lhs;
  }

  /**
   * @brief Bitwise XOR between two flag masks.
   * @param lhs Left-hand mask.
   * @param rhs Right-hand mask.
   * @return XORed mask.
   */
  friend constexpr Flags operator^(Flags lhs, Flags rhs) noexcept {
    lhs ^= rhs;
    return lhs;
  }

  /**
   * @brief Bitwise NOT of a flag mask.
   * @param v Mask to invert.
   * @return Inverted mask.
   * @note Flips all bits in the underlying storage type; it does not mask off unused bits.
   */
  friend constexpr Flags operator~(Flags v) noexcept { return Flags{static_cast<underlying_type>(~v.bits_)}; }

  /** @brief Equality comparison (defaulted). */
  friend constexpr bool operator==(Flags, Flags) noexcept = default;
};

/**
 * @brief Combine two enum flags with bitwise OR into a @ref kEn::util::Flags mask.
 * @tparam E Enum type.
 * @param lhs Left-hand flag.
 * @param rhs Right-hand flag.
 * @return A flag mask containing the combined bits.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator|(E lhs, E rhs) noexcept {
  Flags<E> out{lhs};
  out |= rhs;
  return out;
}

/**
 * @brief Combine two enum flags with bitwise AND into a @ref kEn::util::Flags mask.
 * @tparam E Enum type.
 * @param lhs Left-hand flag.
 * @param rhs Right-hand flag.
 * @return A flag mask containing the intersected bits.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator&(E lhs, E rhs) noexcept {
  Flags<E> out{lhs};
  out &= rhs;
  return out;
}

/**
 * @brief Combine two enum flags with bitwise XOR into a @ref kEn::util::Flags mask.
 * @tparam E Enum type.
 * @param lhs Left-hand flag.
 * @param rhs Right-hand flag.
 * @return A flag mask containing the XORed bits.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator^(E lhs, E rhs) noexcept {
  Flags<E> out{lhs};
  out ^= rhs;
  return out;
}

/**
 * @brief Bitwise NOT of a single enum flag, returning a @ref kEn::util::Flags mask.
 * @tparam E Enum type.
 * @param v Flag to invert.
 * @return A flag mask with all bits flipped in the underlying storage type.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator~(E v) noexcept {
  return ~Flags<E>{v};
}

/**
 * @brief Bitwise OR between a mask and a single enum flag.
 * @tparam E Enum type.
 * @param lhs Left-hand mask.
 * @param rhs Right-hand flag.
 * @return Combined mask.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator|(Flags<E> lhs, E rhs) noexcept {
  lhs |= rhs;
  return lhs;
}

/**
 * @brief Bitwise AND between a mask and a single enum flag.
 * @tparam E Enum type.
 * @param lhs Left-hand mask.
 * @param rhs Right-hand flag.
 * @return Intersected mask.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator&(Flags<E> lhs, E rhs) noexcept {
  lhs &= rhs;
  return lhs;
}

/**
 * @brief Bitwise XOR between a mask and a single enum flag.
 * @tparam E Enum type.
 * @param lhs Left-hand mask.
 * @param rhs Right-hand flag.
 * @return XORed mask.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator^(Flags<E> lhs, E rhs) noexcept {
  lhs ^= rhs;
  return lhs;
}

/**
 * @brief Bitwise OR between a single enum flag and a mask.
 * @tparam E Enum type.
 * @param lhs Left-hand flag.
 * @param rhs Right-hand mask.
 * @return Combined mask.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator|(E lhs, Flags<E> rhs) noexcept {
  rhs |= lhs;
  return rhs;
}

/**
 * @brief Bitwise AND between a single enum flag and a mask.
 * @tparam E Enum type.
 * @param lhs Left-hand flag.
 * @param rhs Right-hand mask.
 * @return Intersected mask.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator&(E lhs, Flags<E> rhs) noexcept {
  rhs &= lhs;
  return rhs;
}

/**
 * @brief Bitwise XOR between a single enum flag and a mask.
 * @tparam E Enum type.
 * @param lhs Left-hand flag.
 * @param rhs Right-hand mask.
 * @return XORed mask.
 * @note Enabled only when @ref kEn::util::enable_flags_v<E> is true.
 */
template <EnumFlag E>
  requires enable_flags_v<E>
[[nodiscard]] constexpr Flags<E> operator^(E lhs, Flags<E> rhs) noexcept {
  rhs ^= lhs;
  return rhs;
}

}  // namespace kEn::util

/**
 * @def KEN_ENABLE_FLAGS(EnumType)
 * @brief Enable enum-flag operators for @p EnumType.
 *
 * Expands to a specialization of @ref kEn::util::enable_flags for @p EnumType and brings the operators
 * from @ref kEn::util into the current scope via @c using declarations.
 *
 * Typical usage:
 * @code
 * enum class Mode : unsigned { A = 1u << 0, B = 1u << 1 };
 * KEN_ENABLE_FLAGS(Mode);
 * @endcode
 *
 * @note Invoke this macro at namespace scope (not inside a class or function).
 */
#ifndef KEN_ENABLE_FLAGS
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define KEN_ENABLE_FLAGS(EnumType)                                \
  template <>                                                     \
  struct ::kEn::util::enable_flags<EnumType> : std::true_type {}; \
  using ::kEn::util::operator|;                                   \
  using ::kEn::util::operator&;                                   \
  using ::kEn::util::operator^;                                   \
  using ::kEn::util::operator~
#endif