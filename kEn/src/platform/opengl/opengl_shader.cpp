#include "kenpch.h"
#include "opengl_shader.h"

#include "glm/gtc/type_ptr.hpp"
#include "kEn/core/assert.h"

#include <filesystem>

namespace kEn
{
	const std::filesystem::path opengl_shader::vertex_ext(".vert");
	const std::filesystem::path opengl_shader::fragment_ext(".frag");


	GLuint opengl_shader::create_shader(const std::string& src, GLint type)
	{
		const GLuint shader = glCreateShader(type);

		if(shader == 0)
		{
			KEN_CORE_ERROR("Shader construction failed!");
			//TODO: maybe throw sth??
			return 0;
		}
		
		const GLchar* source = src.c_str();
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		GLint status = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			std::vector<GLchar> info(length);
			glGetShaderInfoLog(shader, length, &length, info.data());

			glDeleteShader(shader);

			KEN_CORE_ERROR("{0}", info.data());
			KEN_CORE_ASSERT(false, "Shader compilation failed!");

			return 0;
		}

		return shader;
	}

	void opengl_shader::link_shader() const
	{
		glLinkProgram(renderer_id_);

		GLint status = 0;
		glGetProgramiv(renderer_id_, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &length);

			std::vector<GLchar> info(length);
			glGetProgramInfoLog(renderer_id_, length, &length, info.data());

			glDeleteProgram(renderer_id_);

			KEN_CORE_ERROR("{0}", info.data());
			KEN_CORE_ASSERT(false, "Shader program linking failed!");
		}

		glValidateProgram(renderer_id_);

		status = 0;
		glGetProgramiv(renderer_id_, GL_VALIDATE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &length);

			std::vector<GLchar> info(length);
			glGetProgramInfoLog(renderer_id_, length, &length, info.data());

			glDeleteProgram(renderer_id_);

			KEN_CORE_ERROR("{0}", info.data());
			KEN_CORE_ASSERT(false, "Shader program validation failed!");
		}
	}

	void opengl_shader::create_program(const std::string& vertex_src, const std::string& fragment_src)
	{
		GLuint vertex_shader = create_shader(vertex_src, GL_VERTEX_SHADER);
		if (vertex_shader == 0)
		{
			return;
		}

		GLuint fragment_shader = create_shader(fragment_src, GL_FRAGMENT_SHADER);
		if (fragment_shader == 0)
		{
			glDeleteShader(vertex_shader);
			return;
		}

		renderer_id_ = glCreateProgram();

		glAttachShader(renderer_id_, vertex_shader);
		glAttachShader(renderer_id_, fragment_shader);

		link_shader();

		glDetachShader(renderer_id_, vertex_shader);
		glDetachShader(renderer_id_, fragment_shader);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	opengl_shader::opengl_shader(const std::filesystem::path& path)
	{
		auto shader_src_path = shader_path / path;
		name_ = shader_src_path.stem().string();

		shader_src_path.replace_extension(vertex_ext);
		std::ifstream vertex(shader_src_path);
		shader_src_path.replace_extension(fragment_ext);
		std::ifstream fragment(shader_src_path);

		std::stringstream vertex_src, fragment_src;

		vertex_src << vertex.rdbuf();
		fragment_src << fragment.rdbuf();

		create_program(vertex_src.str(), fragment_src.str());
	}

	opengl_shader::opengl_shader(std::string name, const std::string& vertex_src,
	                             const std::string& fragment_src)
		: name_(std::move(name))
	{
		create_program(vertex_src, fragment_src);
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

	// <Uniforms>
	GLint opengl_shader::get_uniform_location(const std::string& name) const
	{
		auto it = uniform_locations_.find(name);
		if (it != uniform_locations_.end())
			return it->second;

		const GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		if(location == -1)
		{
			KEN_CORE_ERROR("Unable to find uniform '{0}' in shader '{1}'", name, name_);
			//TODO: maybe throw sth??
			return -1;
		}

		KEN_CORE_DEBUG("Adding new uniform location for '{0}' = {1}", name, location);
		uniform_locations_[name] = location;
		return location;
	}

	void opengl_shader::set_bool(const std::string& name, bool value)
	{
		GLint location = get_uniform_location(name);
		glUniform1i(location, value);
	}

	void opengl_shader::set_int(const std::string& name, int value)
	{
		GLint location = get_uniform_location(name);
		glUniform1i(location, value);
	}

	void opengl_shader::set_int_array(const std::string& name, int* values, uint32_t count)
	{
		GLint location = get_uniform_location(name);
		glUniform1iv(location, count, values);
	}

	void opengl_shader::set_uint(const std::string& name, uint32_t value)
	{
		GLint location = get_uniform_location(name);
		glUniform1ui(location, value);
	}

	void opengl_shader::set_uint_array(const std::string& name, uint32_t* values, uint32_t count)
	{
		GLint location = get_uniform_location(name);
		glUniform1uiv(location, count, values);
	}

	void opengl_shader::set_float(const std::string& name, float value)
	{
		GLint location = get_uniform_location(name);
		glUniform1f(location, value);
	}

	void opengl_shader::set_float2(const std::string& name, const glm::vec2& value)
	{
		GLint location = get_uniform_location(name);
		glUniform2f(location, value.x, value.y);
	}

	void opengl_shader::set_float3(const std::string& name, const glm::vec3& value)
	{
		GLint location = get_uniform_location(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void opengl_shader::set_float4(const std::string& name, const glm::vec4& value)
	{
		GLint location = get_uniform_location(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void opengl_shader::set_mat3(const std::string& name, const glm::mat3& value)
	{
		GLint location = get_uniform_location(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(value));
	}

	void opengl_shader::set_mat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = get_uniform_location(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
	}

	// </Uniforms>
}
