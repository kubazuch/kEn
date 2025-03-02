#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/shader.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_shader.hpp>

namespace kEn {

const std::filesystem::path shader::shader_path("assets/shaders");

std::shared_ptr<shader> shader::create(const std::string& name, const std::string& vertex_src,
                                       const std::string& fragment_src) {
  switch (renderer_api::get_api()) {
    case renderer_api::api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case renderer_api::api::opengl:
      return std::make_shared<opengl_shader>(name, vertex_src, fragment_src);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<shader> shader::create(const std::filesystem::path& path, shader_config config) {
  switch (renderer_api::get_api()) {
    case renderer_api::api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case renderer_api::api::opengl:
      return std::make_shared<opengl_shader>(path, config);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
