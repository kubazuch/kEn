#pragma once
#include <glm/glm.hpp>

#include "light.h"
#include "material.h"

namespace kEn
{
	struct shader_config
	{
		bool geometry = false;
		bool tessellation = false;
		bool compute = false;
	};

	class shader
	{
	public:
		virtual ~shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		// <Uniforms>
		virtual void set_bool(const std::string& name, bool value) = 0;

		virtual void set_int(const std::string& name, int value) = 0;
		virtual void set_int_array(const std::string& name, int* values, uint32_t count) = 0;

		virtual void set_uint(const std::string& name, uint32_t value) = 0;
		virtual void set_uint_array(const std::string& name, uint32_t* values, uint32_t count) = 0;

		virtual void set_float(const std::string& name, float value) = 0;
		virtual void set_float2(const std::string& name, const glm::vec2& value) = 0;
		virtual void set_float3(const std::string& name, const glm::vec3& value) = 0;
		virtual void set_float4(const std::string& name, const glm::vec4& value) = 0;

		virtual void set_mat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void set_mat4(const std::string& name, const glm::mat4& value) = 0;

		virtual void set_material(const std::string& name, const material& value) { value.load(name, *this); }
		virtual void set_light(const std::string& name, const base_light& value) { value.load(name, *this); }
		// </Uniforms>

		virtual const std::string& get_name() = 0;

		static const std::filesystem::path shader_path;
		static std::unique_ptr<shader> create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
		static std::unique_ptr<shader> create(const std::filesystem::path& path, shader_config config = shader_config());
	};
}
