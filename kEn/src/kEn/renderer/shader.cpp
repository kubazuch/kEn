#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/shader.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_shader.hpp>

namespace kEn {

const std::filesystem::path Shader::kShaderPath("assets/shaders");

std::shared_ptr<Shader> Shader::create(std::string_view name, std::string_view vertex_src,
                                       std::string_view fragment_src) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglShader>(name, vertex_src, fragment_src);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<Shader> Shader::create(const std::filesystem::path& path, ShaderConfig config) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglShader>(path, config);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
