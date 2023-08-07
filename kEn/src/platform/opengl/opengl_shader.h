#pragma once
#include "kEn/renderer/shader.h"
#include <glad/glad.h>

namespace kEn
{
	class opengl_shader final : public shader
	{
	public:
		opengl_shader(std::string name, const std::string& vertex_src,
		              const std::string& fragment_src);
		~opengl_shader() override;

		void bind() const override;
		void unbind() const override;

		void set_int(const std::string& name, int value) override;
		void set_int_array(const std::string& name, int* values, uint32_t count) override;
		void set_float(const std::string& name, float value) override;
		void set_float2(const std::string& name, const glm::vec2& value) override;
		void set_float3(const std::string& name, const glm::vec3& value) override;
		void set_float4(const std::string& name, const glm::vec4& value) override;
		void set_mat4(const std::string& name, const glm::mat4& value) override;

		const std::string& get_name() override { return name_; }

	private:
		uint32_t renderer_id_;

		std::string name_;
	};
}
