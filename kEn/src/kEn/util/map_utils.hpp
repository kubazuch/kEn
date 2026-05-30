#pragma once

#include <memory>
#include <type_traits>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn::util {

/**
 * @brief Return the value mapped to @p key, or @c nullptr if absent.
 *
 * Two overloads selected on @c mapped_type:
 * - Pointer-valued map: returns the stored pointer (@c V) or @c nullptr.
 * - Value map: returns a pointer to the stored value (@c V*) or @c nullptr;
 *   the pointer is valid as long as @p map is not modified.
 *
 * @param map  Associative container (any type with @c find() and @c mapped_type).
 * @param key  Key to look up.
 */
/**
 * Pointer-valued map overload: returns the stored pointer directly, or @c nullptr.
 * @code
 * Derived* d = kEn::util::find_or_null(registry, id);
 * @endcode
 */
template <class Map, class Key>
  requires std::is_pointer_v<typename std::remove_cvref_t<Map>::mapped_type>
auto find_or_null(Map& map, const Key& key) -> typename std::remove_cvref_t<Map>::mapped_type {
  auto it = map.find(key);
  return it != map.end() ? it->second : nullptr;
}

/**
 * Value map overload: returns a pointer to the stored value, or @c nullptr.
 * The returned pointer is valid as long as @p map is not modified.
 * @code
 * auto* sp = kEn::util::find_or_null(shaders, ShapeType::Torus);
 * if (sp) { (*sp)->bind(); }
 * @endcode
 */
template <class Map, class Key>
  requires(!std::is_pointer_v<typename std::remove_cvref_t<Map>::mapped_type>)
auto find_or_null(Map& map, const Key& key) {
  auto it = map.find(key);
  return it != map.end() ? std::addressof(it->second) : nullptr;
}

/**
 * @brief Return the value mapped to @p key, or @p def if absent.
 *
 * Returns by value; prefer @ref find_or_null when a pointer to the stored value
 * is needed or when copying is expensive.
 *
 * @code
 * ShapeType t = kEn::util::find_or(type_map, obj_id, ShapeType::Point);
 * @endcode
 *
 * @param map  Associative container to search.
 * @param key  Key to look up.
 * @param def  Value to return when the key is absent.
 * @return Copy of the mapped value, or @p def.
 */
template <class Map, class Key, class Default>
auto find_or(const Map& map, const Key& key, Default def) -> decltype(map.find(key)->second) {
  auto it = map.find(key);
  return it != map.end() ? it->second : static_cast<decltype(it->second)>(def);
}

}  // namespace kEn::util
