#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include <kEn/util/enum_set.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn::util {

/**
 * @brief Mutable runtime array indexed by a dense, 0-based enum.
 *
 * Wraps @c std::array<V, kSize> where @c kSize = @c std::to_underlying(E::Count).
 * Provides O(1) enum-keyed access and supports runtime mutation, unlike the immutable
 * @ref kEn::util::EnumMap which is @c consteval-only.
 *
 * Use this when the mapped values are constructed or updated at runtime
 * (e.g. a per-enum-value cache, a table of pointers populated during initialization).
 *
 * @tparam E Enum type (must satisfy @ref kEn::util::DenseEnum).
 * @tparam V Mapped value type; must be default-constructible.
 *
 * @code
 * enum class Stage : uint8_t { Vertex, Fragment, Count };
 * EnumArray<Stage, std::string> names;
 * names[Stage::Vertex]   = "vert";
 * names[Stage::Fragment] = "frag";
 * @endcode
 */
template <DenseEnum E, class V>
class EnumArray {
 public:
  /** @brief The enum type used as keys. */
  using enum_type = std::remove_cvref_t<E>;
  /** @brief The mapped value type. */
  using value_type = V;
  /** @brief Number of elements (one per enum value, excluding @c Count). */
  static constexpr std::size_t kSize = static_cast<std::size_t>(std::to_underlying(enum_type::Count));

 private:
  std::array<V, kSize> values_{};

  static constexpr std::size_t to_index(enum_type e) noexcept {
    return static_cast<std::size_t>(std::to_underlying(e));
  }

 public:
  /** @brief Default-construct all values. */
  constexpr EnumArray() = default;

  /**
   * @brief Access the value mapped to @p e.
   * @param e Enum key.
   * @return Reference to the stored value.
   */
  constexpr V& operator[](enum_type e) noexcept { return values_[to_index(e)]; }

  /**
   * @brief Access the value mapped to @p e (read-only).
   * @param e Enum key.
   * @return Const reference to the stored value.
   */
  constexpr const V& operator[](enum_type e) const noexcept { return values_[to_index(e)]; }

  /** @brief Iterator to the first element (in enum order). */
  constexpr auto begin() noexcept { return values_.begin(); }
  /** @brief Past-the-end iterator. */
  constexpr auto end() noexcept { return values_.end(); }
  /** @brief Const iterator to the first element. */
  constexpr auto begin() const noexcept { return values_.begin(); }
  /** @brief Past-the-end const iterator. */
  constexpr auto end() const noexcept { return values_.end(); }
};

}  // namespace kEn::util
