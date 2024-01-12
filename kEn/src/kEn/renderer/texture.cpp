#include "kenpch.h"
#include "texture.h"
#include "renderer.h"
#include "renderer_api.h"

#include "platform/opengl/opengl_texture.h"

namespace kEn
{
	const std::filesystem::path texture2D::texture_path("assets/textures");
	std::unordered_map<std::filesystem::path, std::shared_ptr<texture2D>> texture2D::loaded_resources_;

	std::shared_ptr<texture2D> texture2D::create(const texture_spec& spec)
	{
		switch (renderer_api::get_api())
		{
		case renderer_api::api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_texture2D>(spec);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}

	std::shared_ptr<texture2D> texture2D::create(const std::filesystem::path& name, const texture_spec& spec)
	{
		std::filesystem::path path = texture_path / name;

		if(const auto it = loaded_resources_.find(path); it != loaded_resources_.end())
			return it->second;

		switch (renderer_api::get_api())
		{
		case renderer_api::api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::api::opengl:
			return loaded_resources_[path] = std::make_shared<opengl_texture2D>(path, spec);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
