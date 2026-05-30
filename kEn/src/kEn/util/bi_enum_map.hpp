#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>

#include <kEn/core/assert.hpp>
#include <kEn/core/core.hpp>
#include <kEn/util/enum_map.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn::util {

/**
 * @brief Compile-time bidirectional lookup table mapping dense, 0-based enum values
 *        to arbitrary values and back.
 *
 * Like @ref kEn::util::EnumMap but also supports efficient reverse lookup
 * (value -> enum).  At construction time a sorted copy of the value->enum pairs
 * is built; @ref from_value then uses:
 *
 * - **Binary search O(log N)** when @c V satisfies @c std::totally_ordered.
 * - **Linear search O(N)** otherwise (equality-only types, non-comparable structs).
 *
 * The search strategy is selected automatically via @c if constexpr -- no
 * template parameter needed.
 *
 * Forward lookup is identical to @ref kEn::util::EnumMap: O(1) array index.
 *
 * @tparam E Enum type (values must be 0-based and contiguous; satisfies @ref IndexableEnum).
 * @tparam V Mapped value type; must satisfy @c std::copyable and @c std::equality_comparable.
 * @tparam N Number of enum entries (deduced from the constructor argument).
 *
 * @code
 * enum class Color : uint8_t { Red, Green, Blue, Count };
 *
 * constexpr BiEnumMap kColorNames{{
 *     std::pair{Color::Red,   "red"},
 *     std::pair{Color::Green, "green"},
 *     std::pair{Color::Blue,  "blue"},
 * }};
 *
 * static_assert(kColorNames[Color::Green] == "green");
 * static_assert(kColorNames.from_value("blue") == Color::Blue);
 * @endcode
 */
template <IndexableEnum E, std::copyable V, std::size_t N>
  requires std::equality_comparable<V>
class BiEnumMap {
 public:
  /** @brief The enum type used as keys. */
  using enum_type = std::remove_cvref_t<E>;
  /** @brief The mapped value type. */
  using value_type = V;
  /** @brief Number of entries. */
  static constexpr std::size_t kSize = N;

  BiEnumMap() = delete;

  /**
   * @brief Construct from a fixed-size C-array of (enum, value) pairs.
   *
   * Performs the same compile-time coverage checks as @ref kEn::util::EnumMap.
   * Additionally, builds a reverse index sorted by value (when @c V is
   * @c totally_ordered) for O(log N) @ref from_value queries.
   *
   * @param pairs Array of mappings; order does not matter.
   */
  consteval explicit BiEnumMap(const std::pair<E, V> (&pairs)[N]) {
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
      forward_[idx] = pairs[i].second;
      reverse_[i]   = {pairs[i].second, static_cast<enum_type>(pairs[i].first)};
      present[idx]  = true;
    }
    for (std::size_t i = 0; i < N; ++i) {
      if (!present[i]) {
        throw "Missing mapping for enum value";
      }
    }

    if constexpr (std::totally_ordered<V>) {
      std::sort(reverse_.begin(), reverse_.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    }
  }

  /**
   * @brief Forward lookup: return the value mapped to @p e.  O(1).
   * @param e Enum key (must be in [0, N)).
   * @return The corresponding mapped value.
   */
  [[nodiscard]] constexpr V operator[](enum_type e) const noexcept {
    const auto idx = static_cast<std::size_t>(std::to_underlying(e));
    KEN_CORE_ASSERT(idx < N, "BiEnumMap index out of range");
    return forward_[idx];
  }

  /**
   * @brief Reverse lookup: find the enum value mapped to @p v.
   *
   * Uses binary search when @c V satisfies @c std::totally_ordered, linear
   * search otherwise.
   *
   * @param v Value to search for.
   * @return The enum whose forward mapping equals @p v, or @c std::nullopt.
   */
  [[nodiscard]] constexpr std::optional<enum_type> from_value(const V& v) const {
    if constexpr (std::totally_ordered<V>) {
      auto it = std::lower_bound(reverse_.begin(), reverse_.end(), v,
                                 [](const auto& p, const V& val) { return p.first < val; });
      if (it != reverse_.end() && it->first == v) {
        return it->second;
      }
      return std::nullopt;
    } else {
      for (const auto& [val, key] : reverse_) {
        if (val == v) {
          return key;
        }
      }
      return std::nullopt;
    }
  }

  /** @brief Forward iterator over (enum, value) pairs in enum order. */
  class iterator {  // NOLINT(readability-identifier-naming)
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = std::pair<enum_type, V>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = void;
    using reference         = value_type;

    constexpr iterator(const BiEnumMap* map, std::size_t index) noexcept : map_(map), index_(index) {}

    [[nodiscard]] constexpr value_type operator*() const noexcept {
      return {static_cast<enum_type>(index_), map_->forward_[index_]};
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
    const BiEnumMap* map_;
    std::size_t index_;
  };

  /** @brief Returns an iterator to the first (enum, value) pair. */
  [[nodiscard]] constexpr iterator begin() const noexcept { return {this, 0}; }
  /** @brief Returns the past-the-end iterator. */
  [[nodiscard]] constexpr iterator end() const noexcept { return {this, N}; }

 private:
  std::array<V, N> forward_{};
  std::array<std::pair<V, enum_type>, N> reverse_{};
};

/**
 * @brief Deduction guide: deduces @c E, @c V, and @c N from a C-array of pairs.
 */
template <typename E, typename V, std::size_t N>
BiEnumMap(const std::pair<E, V> (&)[N]) -> BiEnumMap<E, V, N>;

/**
 * @brief Deduction guide: promotes @c const @c char* values to @c std::string_view.
 *
 * Allows string-literal pair initializers without an explicit cast:
 * @code
 * constexpr BiEnumMap kNames{{ std::pair{MyEnum::Foo, "foo"}, ... }};
 * // deduced as BiEnumMap<MyEnum, std::string_view, N>
 * @endcode
 */
template <typename E, std::size_t N>
BiEnumMap(const std::pair<E, const char*> (&)[N]) -> BiEnumMap<E, std::string_view, N>;

}  // namespace kEn::util
