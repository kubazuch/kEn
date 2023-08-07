#include "kenpch.h"
#include "opengl_shader.h"

#include "kEn/core/assert.h"

namespace kEn
{
	opengl_shader::opengl_shader(std::string name, const std::string& vertex_src,
	                             const std::string& fragment_src)
		: name_(std::move(name))
	{
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = vertex_src.c_str();
		glShaderSource(vertex_shader, 1, &source, nullptr);
		glCompileShader(vertex_shader);

		GLint status = 0;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &length);

			std::vector<GLchar> info(length);
			glGetShaderInfoLog(vertex_shader, length, &length, info.data());

			glDeleteShader(vertex_shader);

			KEN_CORE_ERROR("{0}", info.data());
			KEN_CORE_ASSERT(false, "Vertex shader compilation failed!");
			return;
		}

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragment_src.c_str();
		glShaderSource(fragment_shader, 1, &source, nullptr);
		glCompileShader(fragment_shader);

		status = 0;
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &length);

			std::vector<GLchar> info(length);
			glGetShaderInfoLog(fragment_shader, length, &length, info.data());

			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);

			KEN_CORE_ERROR("{0}", info.data());
			KEN_CORE_ASSERT(false, "Fragment shader compilation failed!");
			return;
		}

		renderer_id_ = glCreateProgram();

		glAttachShader(renderer_id_, vertex_shader);
		glAttachShader(renderer_id_, fragment_shader);

		glLinkProgram(renderer_id_);
		status = 0;
		glGetProgramiv(renderer_id_, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &length);

			std::vector<GLchar> info(length);
			glGetProgramInfoLog(renderer_id_, length, &length, info.data());

			glDeleteProgram(renderer_id_);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);

			KEN_CORE_ERROR("{0}", info.data());
			KEN_CORE_ASSERT(false, "Shader program link failed!");
			return;
		}

		glDetachShader(renderer_id_, vertex_shader);
		glDetachShader(renderer_id_, fragment_shader);
	}

	opengl_shader::~opengl_shader()
	{
		glDeleteProgram(renderer_id_);
	}

	void opengl_shader::bind() const
	{
		glUseProgram(renderer_id_);
	}

	void opengl_shader::unbind() const
	{
		glUseProgram(0);
	}

	void opengl_shader::set_int(const std::string& name, int value)
	{
	}

	void opengl_shader::set_int_array(const std::string& name, int* values, uint32_t count)
	{
	}

	void opengl_shader::set_float(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform1f(location, value);
	}

	void opengl_shader::set_float2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void opengl_shader::set_float3(const std::string& name, const glm::vec3& value)
	{
	}

	void opengl_shader::set_float4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void opengl_shader::set_mat4(const std::string& name, const glm::mat4& value)
	{
	}
}
