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

		program_ = glCreateProgram();

		glAttachShader(program_, vertex_shader);
		glAttachShader(program_, fragment_shader);

		glLinkProgram(program_);
		status = 0;
		glGetProgramiv(program_, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);

			std::vector<GLchar> info(length);
			glGetProgramInfoLog(program_, length, &length, info.data());

			glDeleteProgram(program_);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);

			KEN_CORE_ERROR("{0}", info.data());
			KEN_CORE_ASSERT(false, "Shader program link failed!");
			return;
		}

		glDetachShader(program_, vertex_shader);
		glDetachShader(program_, fragment_shader);
	}

	opengl_shader::~opengl_shader()
	{
		glDeleteProgram(program_);
	}

	void opengl_shader::bind() const
	{
		glUseProgram(program_);
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
	}

	void opengl_shader::set_float2(const std::string& name, const glm::vec2& value)
	{
	}

	void opengl_shader::set_float3(const std::string& name, const glm::vec3& value)
	{
	}

	void opengl_shader::set_float4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(program_, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void opengl_shader::set_mat4(const std::string& name, const glm::mat4& value)
	{
	}
}
