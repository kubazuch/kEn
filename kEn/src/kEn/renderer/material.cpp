#include "kenpch.h"
#include "material.h"
#include "shader.h"

void kEn::material::load(const std::string& name, shader& shader) const
{
	if (ambient_factor.is_dirty())
	{
		shader.set_float(name + ".ka", ambient_factor);
		ambient_factor.clear_dirty();
	}

	if (diffuse_factor.is_dirty())
	{
		shader.set_float(name + ".kd", diffuse_factor);
		diffuse_factor.clear_dirty();
	}

	if (specular_factor.is_dirty())
	{
		shader.set_float(name + ".ks", specular_factor);
		specular_factor.clear_dirty();
	}

	if (shininess_factor.is_dirty())
	{
		shader.set_float(name + ".m", shininess_factor);
		shininess_factor.clear_dirty();
	}
	if (color.is_dirty())
	{
		shader.set_float3(name + ".color", color);
		color.clear_dirty();
	}
}

void kEn::material::set_texture(const std::shared_ptr<kEn::texture>& texture)
{
	texture_ = texture;
}

void kEn::material::set_normal_texture(const std::shared_ptr<kEn::texture>& texture)
{
	normal_texture_ = texture;
}

void kEn::material::bind() const
{
	if (texture_)
		texture_->bind();
	if (normal_texture_)
		normal_texture_->bind(1);
}
