#include "texture.hpp"

#include <filesystem>
#include <memory>
#include <unordered_map>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>

#include <platform/opengl/opengl_texture.hpp>

namespace kEn {

const std::filesystem::path Texture2D::kTexturePath("assets/textures");
std::unordered_map<std::filesystem::path, std::shared_ptr<Texture2D>> Texture2D::loaded_resources_;

std::shared_ptr<Texture2D> Texture2D::create(const TextureSpec& spec) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");  // NOLINT
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglTexture2D>(spec);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");  // NOLINT
  return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::create(const std::filesystem::path& name, const TextureSpec& spec) {
  const std::filesystem::path path = kTexturePath / name;

  if (const auto it = loaded_resources_.find(path); it != loaded_resources_.end()) {
    return it->second;
  }

  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");  // NOLINT
      return nullptr;
    case RendererApi::Api::OpenGL:
      return loaded_resources_[path] = std::make_shared<OpenglTexture2D>(path, spec);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");  // NOLINT
  return nullptr;
}

}  // namespace kEn
