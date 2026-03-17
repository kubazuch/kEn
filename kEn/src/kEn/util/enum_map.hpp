#pragma once

#include <array>
#include <cstddef>
#include <iterator>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>

#include <kEn/core/assert.hpp>
#include <kEn/core/core.hpp>

/** @file
 * @ingroup ken
 */

namespace kEn::util {

/**
 * @brief Concept satisfied by enum types that can index an EnumMap.
 *
 * This concept only checks that @p E is an enum; it does not enforce the 0-based contiguous
 * value requirement stated in @ref EnumMap. That contract is validated at compile time by the
 * @c consteval constructor: a mis-shaped enum (values starting at 1, or with gaps) produces a
 * compile-time error via the out-of-range or missing-entry checks.
 *
 * @tparam E Type to test.
 */
template <typename E>
concept IndexableEnum = std::is_enum_v<std::remove_cvref_t<E>>;

/**
 * @brief Compile-time lookup table mapping dense, 0-based enum values to arbitrary values.
 *
 * Constructed via a @c consteval constructor that validates at compile time that
 * every entry in [0, N) is covered exactly once.  Access is O(1) forward and
 * O(N) reverse.
 *
 * If the enum defines a @c Count sentinel as its last enumerator, the constructor
 * additionally enforces that @c N == std::to_underlying(E::Count), turning a
 * forgotten entry into a compile-time error rather than a silent runtime defect.
 *
 * Typical usage:
 * @code
 * // Without Count sentinel -- N deduced from the pair array.
 * enum class Color : uint8_t { Red, Green, Blue };
 *
 * constexpr EnumMap kColorNames{{
 *     std::pair{Color::Red,   "red"},
 *     std::pair{Color::Green, "green"},
 *     std::pair{Color::Blue,  "blue"},
 * }};
 *
 * // With Count sentinel -- adding a new Color without updating kColorNames
 * // is a compile-time error.
 * enum class Color : uint8_t { Red, Green, Blue, Count };
 * @endcode
 *
 * @tparam E Enum type (values must be 0-based and contiguous).
 * @tparam V Mapped value type; must satisfy @c std::copyable.
 * @tparam N Number of enum entries (deduced from the constructor argument).
 */
template <IndexableEnum E, std::copyable V, std::size_t N>
class EnumMap {
 public:
  /** @brief The enum type used as keys. */
  using enum_type = std::remove_cvref_t<E>;
  /** @brief The mapped value type. */
  using value_type = V;
  /** @brief Number of entries in the map. */
  static constexpr std::size_t kSize = N;

  EnumMap() = delete;

  /**
   * @brief Construct from a fixed-size C-array of (enum, value) pairs.
   *
   * The constructor fires at compile time (@c consteval). It verifies:
   * - If @c E defines a @c Count enumerator: @c N == std::to_underlying(E::Count).
   * - Every enum index in [0, N) is covered exactly once.
   * - No enum value maps to an index outside [0, N).
   *
   * A missing, duplicate, or out-of-range entry is a compile-time error.
   *
   * @param pairs Array of mappings; order does not matter.
   */
  consteval explicit EnumMap(const std::pair<E, V> (&pairs)[N]) {
    if constexpr (requires { E::Count; }) {
      static_assert(N == std::to_underlying(E::Count), "Missing mapping for enum value");
    }

    std::array<bool, N> present{};
    for (std::size_t i = 0; i < N; ++i) {
      const auto idx = static_cast<std::size_t>(std::to_underlying(pairs[i].first));
      if (idx >= N) {
        throw "Enum value is out of range [0, N)";
      }
      if (present[idx]) {
        throw "Duplicate mapping for enum value";
      }
      values_[idx] = pairs[i].second;
      present[idx] = true;
    }

    for (std::size_t i = 0; i < N; ++i) {
      if (!present[i]) {
        throw "Missing mapping for enum value";
      }
    }
  }

  /**
   * @brief Look up the value mapped to @p e.
   * @param e Enum value to look up (must be in [0, N)).
   * @return The corresponding mapped value.
   */
  [[nodiscard]] constexpr V operator[](enum_type e) const noexcept {
    const auto idx = static_cast<std::size_t>(std::to_underlying(e));
    KEN_CORE_ASSERT(idx < N, "EnumMap index out of range");
    return values_[idx];
  }

  /**
   * @brief Reverse lookup: find the first enum entry mapped to @p v.
   *
   * @note The mapping need not be one-to-one; only the lowest-index match is returned.
   * @param v Value to search for.
   * @return The enum value that maps to @p v, or @c std::nullopt if none exists.
   */
  [[nodiscard]] constexpr std::optional<enum_type> from_value(const V& v) const {
    for (std::size_t i = 0; i < N; ++i) {
      if (values_[i] == v) {
        return static_cast<enum_type>(i);
      }
    }
    return std::nullopt;
  }

  /** @brief Forward iterator over @c (enum_value, mapped_value) pairs. */
  class iterator {  // NOLINT(readability-identifier-naming)
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = std::pair<enum_type, V>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = void;
    using reference         = value_type;

    constexpr iterator(const EnumMap* map, std::size_t index) noexcept : map_(map), index_(index) {}

    /** @brief Dereference: returns the (enum, value) pair at the current position. */
    [[nodiscard]] constexpr value_type operator*() const noexcept {
      return {static_cast<enum_type>(index_), map_->values_[index_]};
    }

    constexpr iterator& operator++() noexcept {
      ++index_;
      return *this;
    }

    constexpr iterator operator++(int) noexcept {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend constexpr bool operator==(iterator, iterator) noexcept = default;

   private:
    const EnumMap* map_;
    std::size_t index_;
  };

  /** @brief Returns an iterator to the first (enum, value) pair. */
  [[nodiscard]] constexpr iterator begin() const noexcept { return {this, 0}; }
  /** @brief Returns the past-the-end iterator. */
  [[nodiscard]] constexpr iterator end() const noexcept { return {this, N}; }

 private:
  std::array<V, N> values_ = {};
};

/**
 * @brief Deduction guide: deduces @c E, @c V, and @c N from a C-array of pairs.
 * @tparam E Enum type.
 * @tparam V Mapped value type.
 * @tparam N Array length (number of enum entries).
 */
template <typename E, typename V, std::size_t N>
EnumMap(const std::pair<E, V> (&)[N]) -> EnumMap<E, V, N>;

/**
 * @brief Deduction guide: promotes @c const @c char* values to @c std::string_view.
 *
 * Allows string-literal pair initializers without an explicit @c std::string_view cast:
 * @code
 * constexpr EnumMap kNames{{ std::pair{MyEnum::Foo, "foo"}, ... }};
 * // deduced as EnumMap<MyEnum, std::string_view, N>
 * @endcode
 *
 * @tparam E Enum type.
 * @tparam N Array length (number of enum entries).
 */
template <typename E, std::size_t N>
EnumMap(const std::pair<E, const char*> (&)[N]) -> EnumMap<E, std::string_view, N>;

/**
 * @brief Construct an @ref EnumMap with an explicit value type different from the initializer type.
 *
 * Useful when the initializer pairs carry a type @p T that is convertible to @p V but not identical
 * (e.g. initializing an @c EnumMap<E, int> from @c std::pair<E, short> literals).
 *
 * @tparam V Target mapped value type.
 * @tparam E Enum type (deduced).
 * @tparam T Source value type in the pair array; must satisfy @c std::convertible_to<T, V>.
 * @tparam N Number of entries (deduced from @p pairs).
 * @param pairs Array of (enum, value) mappings; same constraints as the @ref EnumMap constructor.
 * @return An @ref EnumMap<E, V, N> with all entries converted to @p V.
 */
template <typename V, typename E, typename T, std::size_t N>
  requires std::convertible_to<T, V>
consteval auto make_enum_map(const std::pair<E, T> (&pairs)[N]) -> EnumMap<E, V, N> {
  std::pair<E, V> converted[N]{};
  for (std::size_t i = 0; i < N; ++i) {
    converted[i] = {pairs[i].first, static_cast<V>(pairs[i].second)};
  }
  return EnumMap<E, V, N>{converted};
}

}  // namespace kEn::util
