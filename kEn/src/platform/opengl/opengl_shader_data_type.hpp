#pragma once

#include <glad/gl.h>

#include <utility>

#include <kEn/renderer/shader_data_type.hpp>

namespace kEn {

namespace shader_data_type {
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

[[nodiscard]] constexpr GLenum get_opengl_type(ShaderDataType type) { return kOpenglTypes[type]; }

}  // namespace shader_data_type

}  // namespace kEn