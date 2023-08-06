#pragma once
#include <glm/glm.hpp>

namespace kEn
{
	class shader
	{
	public:
		virtual ~shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		//TODO: uniforms
		virtual void set_int(const std::string& name, int value) = 0;
		virtual void set_int_array(const std::string& name, int* values, uint32_t count) = 0;
		virtual void set_float(const std::string& name, float value) = 0;
		virtual void set_float2(const std::string& name, const glm::vec2& value) = 0;
		virtual void set_float3(const std::string& name, const glm::vec3& value) = 0;
		virtual void set_float4(const std::string& name, const glm::vec4& value) = 0;
		virtual void set_mat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& get_name() = 0;

		static std::unique_ptr<shader> create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
	};
}
