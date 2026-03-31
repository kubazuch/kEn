#pragma once

#include <glad/gl.h>

#include <utility>

#include <kEn/renderer/shader_data_type.hpp>

namespace kEn {

/**
 * @brief OpenGL-specific query functions for @ref ShaderDataType.
 *
 * Extends @ref kEn::shader_data_type with GL token lookups needed by the
 * vertex-input backend when calling @c glVertexAttribPointer.
 */
namespace shader_data_type {

namespace detail {

inline constexpr auto kOpenglTypes = util::make_enum_map<GLenum>({
    std::pair{Float, GL_FLOAT},
    std::pair{Float2, GL_FLOAT},
    std::pair{Float3, GL_FLOAT},
    std::pair{Float4, GL_FLOAT},
    std::pair{Mat3, GL_FLOAT},
    std::pair{Mat4, GL_FLOAT},
    std::pair{Int, GL_INT},
    std::pair{Int2, GL_INT},
    std::pair{Int3, GL_INT},
    std::pair{Int4, GL_INT},
    std::pair{Bool, GL_BOOL},
});

}  // namespace detail

/**
 * @brief Return the OpenGL base type token for @p type.
 *
 * Returns the @c GLenum passed as the @c type argument to @c glVertexAttribPointer
 * (e.g. @c GL_FLOAT for all float/matrix variants, @c GL_INT for integer variants).
 *
 * @param type A valid @ref ShaderDataType enumerator (not @c Count).
 * @return The corresponding @c GLenum token.
 */
[[nodiscard]] constexpr GLenum opengl_type(ShaderDataType type) { return detail::kOpenglTypes[type]; }

}  // namespace shader_data_type

}  // namespace kEn