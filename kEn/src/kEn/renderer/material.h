#pragma once
#include "texture.h"
#include "glm/vec3.hpp"

namespace kEn
{
	class shader;

	class material
	{
	public:
		material() = default;

		void set_texture(const std::shared_ptr<texture>& texture);
		void set_normal_texture(const std::shared_ptr<texture>& texture);
		void set_ao_texture(const std::shared_ptr<texture>& texture);
		void set_height_texture(const std::shared_ptr<texture>& texture);

		std::shared_ptr<kEn::texture> texture() const { return texture_; }
		std::shared_ptr<kEn::texture> normal_texture() const { return normal_texture_; }
		std::shared_ptr<kEn::texture> ao_texture() const { return ao_texture_; }
		std::shared_ptr<kEn::texture> height_texture() const { return height_texture_; }

		void bind() const;

	private:
		void load(const std::string& name, shader& shader) const;

		friend class shader;

	public:
		lazy_variable<float> ambient_factor = 0.5f;
		lazy_variable<float> diffuse_factor = 0.5f;
		lazy_variable<float> specular_factor = 0.5f;
		lazy_variable<float> shininess_factor = 50.f;
		lazy_variable<glm::vec3> color = glm::vec3{ 1.f };
	private:
		std::shared_ptr<kEn::texture> texture_;
		std::shared_ptr<kEn::texture> normal_texture_;
		std::shared_ptr<kEn::texture> ao_texture_;
		std::shared_ptr<kEn::texture> height_texture_;
	};
}
