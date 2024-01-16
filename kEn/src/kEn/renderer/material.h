#pragma once
#include <map>

#include "texture.h"
#include "glm/vec3.hpp"

namespace kEn
{
	class shader;

	class material
	{
	public:
		material() = default;

		void set_texture(texture_type_t type, const std::shared_ptr<texture>& texture, int id = 0);
		const std::shared_ptr<texture>& texture(texture_type_t type, int id = 0);

		void load(shader& shader, const std::string& name = "u_Material") const;
		void bind() const;

	private:
		friend class shader;

	public:
		float ambient_factor = 0.5f;
		float diffuse_factor = 0.5f;
		float specular_factor = 0.5f;
		float shininess_factor = 50.f;
		glm::vec3 color = glm::vec3{ 1.f };
	private:
		std::unordered_map<texture_type_t, std::vector<std::shared_ptr<kEn::texture>>> textures_;
	};
}
