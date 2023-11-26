#pragma once
#include "glm/vec3.hpp"
#include "kEn/core/transform.h"
#include "kEn/core/core.h"

namespace kEn
{
	class shader;

	struct attenuation
	{
		lazy_variable<float> constant = 1.0f;
		lazy_variable<float> linear = 1.0f;
		lazy_variable<float> quadratic = 1.0f;

		inline void load(const std::string& name, shader& shader) const;
	};
	
	class base_light
	{
	public:
		base_light() = default;
		virtual ~base_light() = default;

		VIRTUAL_FIVE(base_light);
	protected:
		virtual void load(const std::string& name, shader& shader) const = 0;

		friend class shader;
	public:
		lazy_variable<glm::vec3> color = glm::vec3{ 1.f };
		transform transform;
	};

	class point_light : public base_light
	{
	public:


	private:
		void load(const std::string& name, shader& shader) const override;

		friend class shader;
	public:
		attenuation atten;
	};
}
