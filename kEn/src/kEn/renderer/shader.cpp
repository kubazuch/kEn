#include "shader.hpp"

#include <filesystem>
#include <memory>
#include <string_view>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>

#include <platform/opengl/opengl_shader.hpp>

namespace kEn {

const std::filesystem::path Shader::kShaderPath("assets/shaders");

std::shared_ptr<Shader> Shader::create(std::string_view name, std::string_view vertex_src,
                                       std::string_view fragment_src) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_UNREACHABLE();
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglShader>(name, vertex_src, fragment_src);
  }

  KEN_UNREACHABLE();
}

std::shared_ptr<Shader> Shader::create(const std::filesystem::path& path, ShaderConfig config) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_UNREACHABLE();
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglShader>(path, config);
  }

  KEN_UNREACHABLE();
}

}  // namespace kEn
