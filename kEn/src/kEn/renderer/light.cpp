#include "kenpch.h"
#include "light.h"

#include "shader.h"

namespace kEn
{
	inline void attenuation::load(const std::string& name, shader& shader) const
	{
		if (constant.is_dirty())
		{
			shader.set_float(name + ".constant", constant);
			constant.clear_dirty();
		}
		if (linear.is_dirty())
		{
			shader.set_float(name + ".linear", linear);
			linear.clear_dirty();
		}
		if (quadratic.is_dirty())
		{
			shader.set_float(name + ".quadratic", quadratic);
			quadratic.clear_dirty();
		}
	}

	void point_light::load(const std::string& name, shader& shader) const
	{
		if (color.is_dirty())
		{
			shader.set_float3(name + ".color", color);
			color.clear_dirty();
		}

		atten.load(name + ".atten", shader);

		if(transform.pos().is_dirty())
		{
			shader.set_float3(name + ".pos", transform.pos());
			transform.pos().clear_dirty();
		}
	}
}
