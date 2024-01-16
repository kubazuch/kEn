#include "kenpch.h"
#include "material.h"

#include <ranges>

#include "shader.h"

namespace kEn
{
	void material::set_texture(texture_type_t type, const std::shared_ptr<kEn::texture>& texture, int id)
	{
		auto& texs = textures_[type];
		if (id < texs.size())
		{
			texs[id] = texture;
			return;
		}

		if (id == texs.size())
		{
			texs.push_back(texture);
			return;
		}

		throw std::exception("Textures should be added in order!");
	}

	const std::shared_ptr<texture>& material::texture(texture_type_t type, int id)
	{
		return textures_[type][id];
	}

	void material::load(shader& shader, const std::string& name) const
	{
		shader.set_float(name + ".ka", ambient_factor);
		shader.set_float(name + ".kd", diffuse_factor);
		shader.set_float(name + ".ks", specular_factor);
		shader.set_float(name + ".m", shininess_factor);
		shader.set_float3(name + ".color", color);

		int texture_id = 0;
		for (const auto& [type, textures] : textures_)
		{
			for (int i = 0; i < textures.size(); i++)
			{
				shader.set_int("u_Material." + std::string(texture_type::name_of(type)) + std::to_string(i), i);
				texture_id++;
			}
		}
	}

	void material::bind() const
	{
		int texture_id = 0;
		for(const auto& textures : textures_ | std::views::values)
		{
			for (const auto& texture : textures)
			{
				texture->bind(texture_id);
				texture_id++;
			}
		}
	}

}
