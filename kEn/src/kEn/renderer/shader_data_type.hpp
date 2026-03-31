#pragma once

#include <cstdint>

#include <kEn/core/core.hpp>
#include <kEn/util/enum_map.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Scalar and matrix data types that a shader attribute or uniform can hold.
 *
 * Used to describe the element type of a vertex attribute (via @ref VertexElement) and
 * to query byte sizes and component counts at compile time through
 * @ref shader_data_type::size() and @ref shader_data_type::component_count().
 *
 * @c Count is a sentinel used by @ref util::EnumMap to enforce full coverage of the
 * lookup tables at compile time.
 */
enum class ShaderDataType : std::uint8_t {
  Float,  /**< Single 32-bit float (@c float). */
  Float2, /**< Two-component 32-bit float vector (@c vec2). */
  Float3, /**< Three-component 32-bit float vector (@c vec3). */
  Float4, /**< Four-component 32-bit float vector (@c vec4). */
  Mat3,   /**< Column-major 3x3 float matrix (@c mat3), occupying 9 floats. */
  Mat4,   /**< Column-major 4x4 float matrix (@c mat4), occupying 16 floats. */
  Int,    /**< Single 32-bit signed integer (@c int). */
  Int2,   /**< Two-component 32-bit signed integer vector (@c ivec2). */
  Int3,   /**< Three-component 32-bit signed integer vector (@c ivec3). */
  Int4,   /**< Four-component 32-bit signed integer vector (@c ivec4). */
  Bool,   /**< Boolean value; stored as 4 bytes to match GLSL std140/std430 layout. */
  Count   /**< Sentinel: total number of enumerators. Not a valid data type. */
};

/**
 * @brief Compile-time metadata and accessor functions for @ref ShaderDataType.
 *
 * All public functions perform O(1) table lookups and are usable in constant
 * expressions.  The raw lookup tables reside in the nested @c detail namespace
 * and are not part of the public API.
 */
namespace shader_data_type {

using enum ShaderDataType;

/** @brief Byte size of a single 32-bit float component. */
inline constexpr std::uint32_t kFloatSize = 4;
/** @brief Byte size of a single 32-bit integer component. */
inline constexpr std::uint32_t kIntSize = 4;
/** @brief Byte size of a boolean value as stored in a GLSL std140/std430 block (4 bytes). */
inline constexpr std::uint32_t kBoolSize = 4;

namespace detail {

inline constexpr auto kSizes = util::make_enum_map<std::uint32_t>({
    std::pair{Float, kFloatSize},
    std::pair{Float2, 2 * kFloatSize},
    std::pair{Float3, 3 * kFloatSize},
    std::pair{Float4, 4 * kFloatSize},
    std::pair{Mat3, 9 * kFloatSize},
    std::pair{Mat4, 16 * kFloatSize},
    std::pair{Int, kIntSize},
    std::pair{Int2, 2 * kIntSize},
    std::pair{Int3, 3 * kIntSize},
    std::pair{Int4, 4 * kIntSize},
    std::pair{Bool, kBoolSize},
});

inline constexpr auto kComponents = util::make_enum_map<std::uint8_t>({
    std::pair{Float, 1},
    std::pair{Float2, 2},
    std::pair{Float3, 3},
    std::pair{Float4, 4},
    std::pair{Mat3, 3},
    std::pair{Mat4, 4},
    std::pair{Int, 1},
    std::pair{Int2, 2},
    std::pair{Int3, 3},
    std::pair{Int4, 4},
    std::pair{Bool, 1},
});

}  // namespace detail

/**
 * @brief Return the byte size of @p type.
 *
 * Equivalent to the number of bytes a single value of @p type occupies in a tightly
 * packed vertex buffer or std140/std430 UBO (before any alignment padding).
 *
 * @param type A valid @ref ShaderDataType enumerator (not @c Count).
 * @return Byte size; e.g. 4 for @c Float, 48 for @c Mat3.
 */
[[nodiscard]] constexpr std::uint32_t size(ShaderDataType type) { return detail::kSizes[type]; }

/**
 * @brief Return the number of scalar components in @p type.
 *
 * For vector types this is the vector length; for matrix types it is the column
 * length (not the total element count).  Used by vertex-input backends to supply
 * the @c size argument to @c glVertexAttribPointer.
 *
 * @param type A valid @ref ShaderDataType enumerator (not @c Count).
 * @return Component count; e.g. 1 for @c Float/@c Int, 4 for @c Float4/@c Mat4.
 */
[[nodiscard]] constexpr std::uint8_t component_count(ShaderDataType type) { return detail::kComponents[type]; }

}  // namespace shader_data_type

}  // namespace kEn