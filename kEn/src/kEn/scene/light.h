#pragma once
#include "glm/vec3.hpp"
#include "kEn/core/transform.h"
#include "kEn/core/core.h"

namespace kEn
{
	class shader;

	struct attenuation
	{
		float constant = 1.0f;
		float linear = 0.0f;
		float quadratic = 0.0f;

		inline void load(const std::string& name, shader& shader) const;
	};
	
	class base_light
	{
	public:
		base_light() = default;
		virtual ~base_light() = default;
		virtual void imgui(bool subsection) = 0;

		VIRTUAL_FIVE(base_light);
	protected:
		virtual void load(const std::string& name, shader& shader) const = 0;

		friend class shader;
	public:
		glm::vec3 color = glm::vec3{ 1.f };
		transform transform;
	};

	class directional_light : public base_light
	{
	public:
		void imgui(bool subsection = true) override;

	private:
		void load(const std::string& name, shader& shader) const override;

	private:
		friend class shader;
	};

	class point_light : public base_light
	{
	public:
		void imgui(bool subsection = true) override;

	private:
		void load(const std::string& name, shader& shader) const override;

		friend class shader;
	public:
		attenuation atten;
	};

	class spot_light : public base_light
	{
	public:
		void imgui(bool subsection = true) override;

	private:
		void load(const std::string& name, shader& shader) const override;

		friend class shader;
	public:
		attenuation atten;
		float inner_cutoff_angle = 0.1f;
		float outer_cutoff_angle = 0.1f;
	};
}
