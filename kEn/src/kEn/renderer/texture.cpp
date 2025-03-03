#include <kEn/renderer/renderer.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/texture.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_texture.hpp>

namespace kEn {

const std::filesystem::path Texture2D::kTexturePath("assets/textures");
std::unordered_map<std::filesystem::path, std::shared_ptr<Texture2D>> Texture2D::loaded_resources_;

std::shared_ptr<Texture2D> Texture2D::create(const TextureSpec& spec) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglTexture2D>(spec);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::create(const std::filesystem::path& name, const TextureSpec& spec) {
  std::filesystem::path path = kTexturePath / name;

  if (const auto it = loaded_resources_.find(path); it != loaded_resources_.end()) {
    return it->second;
  }

  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return loaded_resources_[path] = std::make_shared<OpenglTexture2D>(path, spec);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
