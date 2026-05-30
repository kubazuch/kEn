#pragma once

#include <bitset>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn::util {

/**
 * @brief Concept satisfied by enum types with a @c Count sentinel.
 *
 * Requires the type to be an enum and to define a @c Count enumerator as its last value.
 * @c Count is used to determine the capacity of the @ref kEn::util::EnumSet at compile time.
 *
 * Typical usage:
 * @code
 * enum class Color : uint8_t { Red, Green, Blue, Count };
 * static_assert(DenseEnum<Color>);
 * @endcode
 *
 * @tparam E Type to test.
 */
template <typename E>
concept DenseEnum = std::is_enum_v<std::remove_cvref_t<E>> && requires { std::remove_cvref_t<E>::Count; };

/**
 * @brief Type-safe set over a dense, 0-based enum.
 *
 * Stores membership as a @c std::bitset indexed by the enum's underlying value.
 * The enum must have a @c Count sentinel as its last enumerator; the bitset capacity
 * is @c std::to_underlying(E::Count).
 *
 * Complements @ref kEn::util::Flags, which is intended for power-of-two bitmask enums.
 * Use @c EnumSet when enum values are sequential indices and combining values with @c |
 * is not meaningful on the enum itself.
 *
 * @tparam E Enum type (must satisfy @ref kEn::util::DenseEnum).
 *
 * Example:
 * @code
 * enum class Feature : uint8_t { Shadows, Bloom, Ssao, Count };
 *
 * EnumSet<Feature> enabled;
 * enabled.set(Feature::Shadows).set(Feature::Bloom);
 * if (enabled.test(Feature::Bloom)) { ... }
 * @endcode
 */
template <DenseEnum E>
class EnumSet {
 public:
  /** @brief The enum type represented by this set. */
  using enum_type = std::remove_cvref_t<E>;
  /** @brief Number of distinct enum values (capacity of the underlying bitset). */
  static constexpr std::size_t kSize = static_cast<std::size_t>(std::to_underlying(enum_type::Count));

 private:
  std::bitset<kSize> bits_;

  static constexpr std::size_t to_index(enum_type e) noexcept {
    return static_cast<std::size_t>(std::to_underlying(e));
  }

 public:
  /** @brief Construct an empty set (no values present). */
  constexpr EnumSet() noexcept = default;

  /**
   * @brief Construct a set containing a single value.
   * @param e The value to insert.
   */
  constexpr explicit EnumSet(enum_type e) noexcept { bits_.set(to_index(e)); }

  /**
   * @brief Construct a set from an initializer list of values.
   * @param init Values to insert.
   */
  constexpr EnumSet(std::initializer_list<enum_type> init) noexcept {
    for (enum_type e : init) {
      bits_.set(to_index(e));
    }
  }

  /** @brief @return True if any value is present. */
  [[nodiscard]] constexpr bool any() const noexcept { return bits_.any(); }
  /** @brief @return True if no value is present. */
  [[nodiscard]] constexpr bool none() const noexcept { return bits_.none(); }
  /** @brief @return True if all @c kSize values are present. */
  [[nodiscard]] constexpr bool all() const noexcept { return bits_.all(); }
  /** @brief Explicit boolean conversion -- same as @ref any(). */
  [[nodiscard]] constexpr explicit operator bool() const noexcept { return any(); }
  /** @brief @return Number of values currently in the set. */
  [[nodiscard]] constexpr std::size_t count() const noexcept { return bits_.count(); }

  /**
   * @brief Test whether @p e is in the set.
   * @param e Value to test.
   * @return True if @p e is present.
   */
  [[nodiscard]] constexpr bool test(enum_type e) const noexcept { return bits_.test(to_index(e)); }

  /**
   * @brief Insert @p e into the set.
   * @param e Value to insert.
   * @return @c *this.
   */
  constexpr EnumSet& set(enum_type e) noexcept {
    bits_.set(to_index(e));
    return *this;
  }

  /**
   * @brief Remove @p e from the set.
   * @param e Value to remove.
   * @return @c *this.
   */
  constexpr EnumSet& reset(enum_type e) noexcept {
    bits_.reset(to_index(e));
    return *this;
  }

  /**
   * @brief Toggle the presence of @p e.
   * @param e Value to toggle.
   * @return @c *this.
   */
  constexpr EnumSet& toggle(enum_type e) noexcept {
    bits_.flip(to_index(e));
    return *this;
  }

  /**
   * @brief Remove all values from the set.
   * @return @c *this.
   */
  constexpr EnumSet& clear() noexcept {
    bits_.reset();
    return *this;
  }

  /**
   * @brief Check whether all values in @p rhs are also in @c *this.
   * @param rhs Set of values to test.
   * @return True if @c *this is a superset of @p rhs.
   */
  [[nodiscard]] constexpr bool contains_all(EnumSet rhs) const noexcept { return (bits_ & rhs.bits_) == rhs.bits_; }

  /**
   * @brief Check whether any value from @p rhs is in @c *this.
   * @param rhs Set of values to test.
   * @return True if the intersection of @c *this and @p rhs is non-empty.
   */
  [[nodiscard]] constexpr bool contains_any(EnumSet rhs) const noexcept { return (bits_ & rhs.bits_).any(); }

  /**
   * @brief Invoke a callback for every value present in the set, in ascending enum order.
   * @tparam Fn Callable type accepting @c enum_type.
   * @param fn Callback to invoke.
   */
  template <typename Fn>
  constexpr void for_each_set(Fn&& fn) const noexcept(noexcept(std::forward<Fn>(fn)(enum_type{}))) {
    for (std::size_t i = 0; i < kSize; ++i) {
      if (bits_.test(i)) {
        std::forward<Fn>(fn)(static_cast<enum_type>(i));
      }
    }
  }

  /** @brief In-place union with @p rhs. */
  constexpr EnumSet& operator|=(EnumSet rhs) noexcept {
    bits_ |= rhs.bits_;
    return *this;
  }
  /** @brief In-place intersection with @p rhs. */
  constexpr EnumSet& operator&=(EnumSet rhs) noexcept {
    bits_ &= rhs.bits_;
    return *this;
  }
  /** @brief In-place symmetric difference with @p rhs. */
  constexpr EnumSet& operator^=(EnumSet rhs) noexcept {
    bits_ ^= rhs.bits_;
    return *this;
  }

  /** @brief In-place insert of a single value (union with singleton set). */
  constexpr EnumSet& operator|=(enum_type e) noexcept { return (*this |= EnumSet{e}); }
  /** @brief In-place mask to a single value (intersection with singleton set). */
  constexpr EnumSet& operator&=(enum_type e) noexcept { return (*this &= EnumSet{e}); }
  /** @brief In-place toggle of a single value (symmetric difference with singleton set). */
  constexpr EnumSet& operator^=(enum_type e) noexcept { return (*this ^= EnumSet{e}); }

  /** @brief Union of two sets. */
  friend constexpr EnumSet operator|(EnumSet lhs, EnumSet rhs) noexcept {
    lhs |= rhs;
    return lhs;
  }
  /** @brief Intersection of two sets. */
  friend constexpr EnumSet operator&(EnumSet lhs, EnumSet rhs) noexcept {
    lhs &= rhs;
    return lhs;
  }
  /** @brief Symmetric difference of two sets. */
  friend constexpr EnumSet operator^(EnumSet lhs, EnumSet rhs) noexcept {
    lhs ^= rhs;
    return lhs;
  }
  /**
   * @brief Complement -- the set of all values NOT in @p v.
   * @warning Flips all @c kSize bits; values outside the enum range are not representable
   *          and thus do not appear in the result.
   */
  friend constexpr EnumSet operator~(EnumSet v) noexcept {
    v.bits_.flip();
    return v;
  }

  /** @brief Equality comparison. */
  friend constexpr bool operator==(const EnumSet&, const EnumSet&) noexcept = default;
};

}  // namespace kEn::util
