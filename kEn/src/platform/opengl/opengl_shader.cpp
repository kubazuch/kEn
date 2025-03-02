#include "kenpch.h"
#include "opengl_shader.h"

#include "glm/gtc/type_ptr.hpp"
#include "kEn/core/assert.h"

#include <filesystem>

namespace kEn
{
	const std::filesystem::path opengl_shader::vertex_ext(".vert");
	const std::filesystem::path opengl_shader::fragment_ext(".frag");
	const std::filesystem::path opengl_shader::geometry_ext(".geom");
	const std::filesystem::path opengl_shader::tess_control_ext(".tesc");
	const std::filesystem::path opengl_shader::tess_eval_ext(".tese");
	const std::regex opengl_shader::include_regex("#include\\s+\"(.+)\"");

	std::string opengl_shader::read_shader_src_internal(const std::filesystem::path& filePath, std::unordered_set<std::filesystem::path>& included, bool internal)
	{
		if (included.find(filePath) != included.end())
		{
			if(!internal)
				KEN_CORE_WARN("Circular dependency or already loaded package detected in file {0}!", filePath.string());
			return "";
		}

		included.insert(filePath);

		std::unique_ptr<std::istream> stream;
		if (internal)
		{
			auto it = internal_libs.find(filePath.string());
			stream = std::make_unique<std::stringstream>(it->second);
		}
		else
		{
			auto file = std::make_unique<std::ifstream>(filePath);
			if (!file->is_open()) {
				std::cerr << "Error opening file: " << filePath << std::endl;
				return "";
			}

			stream = std::move(file);
		}

		std::stringstream buffer;
		std::string line;
		while (std::getline(*stream, line)) {
			std::smatch includeMatch;
			if (std::regex_match(line, includeMatch, include_regex)) {
				std::string includeFileName = includeMatch[1].str();
				std::string includedSource;
				if (auto it = internal_libs.find(includeFileName); it != internal_libs.end())
				{
					includedSource = read_shader_src_internal(includeFileName, included, true);
				}
				else
				{
					includedSource = read_shader_src_internal(shader_path / includeFileName, included);
				}
				buffer << includedSource << "\n";
			}
			else
			{
				buffer << line << "\n";
			}
		}

		return buffer.str();
	}

	std::string opengl_shader::read_shader_src(const std::filesystem::path& file)
	{
		std::unordered_set<std::filesystem::path> includedFiles;
		return read_shader_src_internal(file, includedFiles);
	}

	GLuint opengl_shader::create_shader(const std::string& src, GLint type)
	{
		const GLuint shader = glCreateShader(type);

		if (shader == 0)
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

	opengl_shader::opengl_shader(const std::filesystem::path& path, shader_config config)
	{
		auto shader_src_path = shader_path / path;
		name_ = shader_src_path.stem().string();

		GLuint vertex_shader = 0, fragment_shader = 0;
		GLuint geometry_shader = 0;
		GLuint tess_control_shader = 0, tess_evaluation_shader = 0;

		// Vertex

		shader_src_path.replace_extension(vertex_ext);
		vertex_shader = create_shader(read_shader_src(shader_src_path), GL_VERTEX_SHADER);
		if (vertex_shader == 0)
			throw std::exception("Unable to create vertex shader");

		// Fragment
		shader_src_path.replace_extension(fragment_ext);
		fragment_shader = create_shader(read_shader_src(shader_src_path), GL_FRAGMENT_SHADER);
		if (fragment_shader == 0)
		{
			glDeleteShader(vertex_shader);
			throw std::exception("Unable to create fragment shader");
		}

		// Geometry
		if (config.geometry)
		{
			shader_src_path.replace_extension(geometry_ext);
			geometry_shader = create_shader(read_shader_src(shader_src_path), GL_GEOMETRY_SHADER);
			if (geometry_shader == 0)
			{
				glDeleteShader(vertex_shader);
				glDeleteShader(fragment_shader);
				throw std::exception("Unable to create geometry shader");
			}
		}

		// Tessellation
		if (config.tessellation)
		{
			shader_src_path.replace_extension(tess_control_ext);
			tess_control_shader = create_shader(read_shader_src(shader_src_path), GL_TESS_CONTROL_SHADER);
			if (tess_control_shader == 0)
			{
				glDeleteShader(vertex_shader);
				glDeleteShader(fragment_shader);
				glDeleteShader(geometry_shader);
				throw std::exception("Unable to create tessellation control shader");
			}

			shader_src_path.replace_extension(tess_eval_ext);
			tess_evaluation_shader = create_shader(read_shader_src(shader_src_path), GL_TESS_EVALUATION_SHADER);
			if (tess_evaluation_shader == 0)
			{
				glDeleteShader(vertex_shader);
				glDeleteShader(fragment_shader);
				glDeleteShader(geometry_shader);
				glDeleteShader(tess_control_shader);
				throw std::exception("Unable to create tessellation evaluation shader");
			}
		}

		renderer_id_ = glCreateProgram();

		glAttachShader(renderer_id_, vertex_shader);
		glAttachShader(renderer_id_, fragment_shader);
		if (config.geometry)
			glAttachShader(renderer_id_, geometry_shader);

		if (config.tessellation)
		{
			glAttachShader(renderer_id_, tess_control_shader);
			glAttachShader(renderer_id_, tess_evaluation_shader);
		}

		link_shader();

		glDetachShader(renderer_id_, vertex_shader);
		glDetachShader(renderer_id_, fragment_shader);
		if (config.geometry)
			glDetachShader(renderer_id_, geometry_shader);
		if (config.tessellation)
		{
			glDetachShader(renderer_id_, tess_control_shader);
			glDetachShader(renderer_id_, tess_evaluation_shader);
		}

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		if (config.geometry)
			glDeleteShader(geometry_shader);
		if (config.tessellation)
		{
			glDeleteShader(tess_control_shader);
			glDeleteShader(tess_evaluation_shader);
		}
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
		uniform_locations_[name] = location;
		if (location == -1)
		{
			KEN_CORE_ERROR("Unable to find uniform '{0}' in shader '{1}'", name, name_);
			//TODO: maybe throw sth??
			return -1;
		}

		KEN_CORE_DEBUG("Adding new uniform location for '{0}' = {1}", name, location);
		return location;
	}

	void opengl_shader::set_bool(const std::string& name, bool value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform1i(renderer_id_, location, value);
	}

	void opengl_shader::set_int(const std::string& name, int value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform1i(renderer_id_, location, value);
	}

	void opengl_shader::set_int_array(const std::string& name, int* values, uint32_t count)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform1iv(renderer_id_, location, count, values);
	}

	void opengl_shader::set_uint(const std::string& name, uint32_t value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform1ui(renderer_id_, location, value);
	}

	void opengl_shader::set_uint_array(const std::string& name, uint32_t* values, uint32_t count)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform1uiv(renderer_id_, location, count, values);
	}

	void opengl_shader::set_float(const std::string& name, float value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform1f(renderer_id_, location, value);
	}

	void opengl_shader::set_float2(const std::string& name, const glm::vec2& value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform2f(renderer_id_, location, value.x, value.y);
	}

	void opengl_shader::set_float3(const std::string& name, const glm::vec3& value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform3f(renderer_id_, location, value.x, value.y, value.z);
	}

	void opengl_shader::set_float4(const std::string& name, const glm::vec4& value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniform4f(renderer_id_, location, value.x, value.y, value.z, value.w);
	}

	void opengl_shader::set_mat3(const std::string& name, const glm::mat3& value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniformMatrix3fv(renderer_id_, location, 1, GL_FALSE, value_ptr(value));
	}

	void opengl_shader::set_mat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = get_uniform_location(name);
		glProgramUniformMatrix4fv(renderer_id_, location, 1, GL_FALSE, value_ptr(value));
	}

	// </Uniforms>

	const std::unordered_map<std::string, std::string> opengl_shader::internal_libs = {
		{"material",
R"(const int MAX_TEXTURES = 5;

struct material {
	float ka;
	float kd;
	float ks;
	float m;

	bool emissive;

	int diffuse_count;
	sampler2D diffuse[MAX_TEXTURES];
};)"},
		{"light",
R"(#include "material"
struct attenuation {
	float constant;
	float linear;
	float quadratic;
};

struct directional_light {
	vec3 color;

	vec3 dir;
}; 

struct point_light {
	vec3 color;

	attenuation atten;

	vec3 pos;
}; 

struct spot_light {
	vec3 color;

	attenuation atten;

	vec3 pos;
	vec3 dir;
	float cutoff;
	float outerCutoff;
}; 

const int MAX_LIGHTS = 15;

uniform bool u_UseBlinn = false;

float calc_attenuation(attenuation atten, float dist) {
	return 1.0 / (atten.constant + dist * (atten.linear + dist * atten.quadratic));
}

vec2 calc_light(vec3 light_dir, vec3 normal, vec3 view_dir) {
	float diff = max(dot(normal, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = max(dot(view_dir, reflect_dir), 0.0);
	
	return vec2(diff, spec);
}

vec2 calc_light_blinn(vec3 light_dir, vec3 normal, vec3 view_dir) {
	float diff = max(dot(normal, light_dir), 0.0);

	vec3 halfway_dir = normalize(light_dir + view_dir);
	float spec = max(dot(normal, halfway_dir), 0.0);
	
	return vec2(diff, spec);
}

vec3 calc_point_light(point_light light, material mat, vec3 normal, vec3 frag_pos, vec3 view_dir) {
	vec2 factors = u_UseBlinn ? calc_light_blinn(normalize(light.pos - frag_pos), normal, view_dir) : calc_light(normalize(light.pos - frag_pos), normal, view_dir);

	vec3 diffuse = mat.kd * factors.x * light.color;
	vec3 specular = mat.ks * pow(factors.y, u_UseBlinn ? 4*mat.m : mat.m) * light.color;

	return (diffuse + specular) * calc_attenuation(light.atten, length(light.pos - frag_pos));
}

vec3 calc_dir_light(directional_light light, material mat, vec3 normal, vec3 view_dir) {
	vec2 factors = u_UseBlinn ? calc_light_blinn(normalize(-light.dir), normal, view_dir) : calc_light(normalize(-light.dir), normal, view_dir);

	vec3 diffuse = mat.kd * factors.x * light.color;
	vec3 specular = mat.ks * pow(factors.y, u_UseBlinn ? 4*mat.m : mat.m) * light.color;

	return (diffuse + specular);
}

vec3 calc_spot_light(spot_light light, material mat, vec3 normal, vec3 frag_pos, vec3 view_dir) {
	vec3 light_dir = normalize(light.pos - frag_pos);

	float theta = dot(light_dir, normalize(-light.dir));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity;
	if(epsilon == 0)
		intensity = theta > light.cutoff ? 1 : 0;
	else
		intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);    

	vec2 factors = u_UseBlinn ? calc_light_blinn(normalize(light.pos - frag_pos), normal, view_dir) : calc_light(normalize(light.pos - frag_pos), normal, view_dir);

	vec3 diffuse = mat.kd * factors.x * light.color;
	vec3 specular = mat.ks * pow(factors.y, u_UseBlinn ? 4*mat.m : mat.m) * light.color;

	return (diffuse + specular) * calc_attenuation(light.atten, length(light.pos - frag_pos)) * intensity;
}
)"},
	};
}
