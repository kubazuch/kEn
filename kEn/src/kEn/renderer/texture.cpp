#include "kenpch.h"
#include "texture.h"
#include "renderer.h"
#include "renderer_api.h"

#include "platform/opengl/opengl_texture.h"

namespace kEn
{
	const std::filesystem::path texture2D::texture_path("assets/textures");

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

	std::shared_ptr<texture2D> texture2D::create(const std::filesystem::path& path)
	{
		switch (renderer_api::get_api())
		{
		case renderer_api::api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_texture2D>(texture_path / path);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
