#include "opengl_shader.hpp"

#include <filesystem>
#include <iostream>
#include <kEn/core/assert.hpp>
#include <kEn/renderer/shader.hpp>
#include <string_view>
#include <unordered_map>

namespace kEn {

const std::filesystem::path OpenglShader::kVertexExt(".vert");
const std::filesystem::path OpenglShader::kFragmentExt(".frag");
const std::filesystem::path OpenglShader::kGeometryExt(".geom");
const std::filesystem::path OpenglShader::kTessControlExt(".tesc");
const std::filesystem::path OpenglShader::kTessEvalExt(".tese");
const std::regex OpenglShader::kIncludeRegex("#include\\s+\"(.+)\"");

std::string OpenglShader::read_shader_src_internal(const std::filesystem::path& filePath,
                                                   std::unordered_set<std::filesystem::path>& included, bool internal) {
  if (included.contains(filePath)) {
    if (!internal) {
      KEN_CORE_WARN("Circular dependency or already loaded package detected in file {0}!", filePath.string());
    }
    return "";
  }

  included.insert(filePath);

  std::unique_ptr<std::istream> stream;
  if (internal) {
    auto it = kInternalLibs.find(filePath.string());
    stream  = std::make_unique<std::stringstream>(std::string(it->second));
  } else {
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
    std::smatch include_match;
    if (std::regex_match(line, include_match, kIncludeRegex)) {
      std::string include_file_name = include_match[1].str();
      std::string included_source;
      if (auto it = kInternalLibs.find(include_file_name); it != kInternalLibs.end()) {
        included_source = read_shader_src_internal(include_file_name, included, true);
      } else {
        included_source = read_shader_src_internal(kShaderPath / include_file_name, included);
      }
      buffer << included_source << "\n";
    } else {
      buffer << line << "\n";
    }
  }

  return buffer.str();
}

std::string OpenglShader::read_shader_src(const std::filesystem::path& file) {
  std::unordered_set<std::filesystem::path> included_files;
  return read_shader_src_internal(file, included_files);
}

GLuint OpenglShader::create_shader(std::string_view src, GLenum type) {
  const GLuint shader = glCreateShader(type);

  if (shader == 0) {
    KEN_CORE_ERROR("Shader construction failed!");
    return 0;
  }

  const GLchar* source = src.data();
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> info(static_cast<size_t>(length));
    glGetShaderInfoLog(shader, length, &length, info.data());

    glDeleteShader(shader);

    KEN_CORE_ERROR("{0}", info.data());
    KEN_CORE_ASSERT(false, "Shader compilation failed!");

    return 0;
  }

  return shader;
}

void OpenglShader::link_shader() const {
  glLinkProgram(renderer_id_);

  GLint status = 0;
  glGetProgramiv(renderer_id_, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint length = 0;
    glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> info(static_cast<size_t>(length));
    glGetProgramInfoLog(renderer_id_, length, &length, info.data());

    glDeleteProgram(renderer_id_);

    KEN_CORE_ERROR("{0}", info.data());
    KEN_CORE_ASSERT(false, "Shader program linking failed!");
  }

  glValidateProgram(renderer_id_);

  status = 0;
  glGetProgramiv(renderer_id_, GL_VALIDATE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint length = 0;
    glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> info(static_cast<size_t>(length));
    glGetProgramInfoLog(renderer_id_, length, &length, info.data());

    glDeleteProgram(renderer_id_);

    KEN_CORE_ERROR("{0}", info.data());
    KEN_CORE_ASSERT(false, "Shader program validation failed!");
  }
}

void OpenglShader::create_program(std::string_view vertex_src, std::string_view fragment_src) {
  GLuint vertex_shader = create_shader(vertex_src, GL_VERTEX_SHADER);
  if (vertex_shader == 0) {
    return;
  }

  GLuint fragment_shader = create_shader(fragment_src, GL_FRAGMENT_SHADER);
  if (fragment_shader == 0) {
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

OpenglShader::OpenglShader(const std::filesystem::path& path, ShaderConfig config) {
  auto shader_src_path = kShaderPath / path;

  name_ = shader_src_path.stem().string();

  GLuint vertex_shader          = 0;
  GLuint fragment_shader        = 0;
  GLuint geometry_shader        = 0;
  GLuint tess_control_shader    = 0;
  GLuint tess_evaluation_shader = 0;

  // Vertex

  shader_src_path.replace_extension(kVertexExt);
  vertex_shader = create_shader(read_shader_src(shader_src_path), GL_VERTEX_SHADER);
  if (vertex_shader == 0) {
    throw std::exception("Unable to create vertex shader");
  }

  // Fragment
  shader_src_path.replace_extension(kFragmentExt);
  fragment_shader = create_shader(read_shader_src(shader_src_path), GL_FRAGMENT_SHADER);
  if (fragment_shader == 0) {
    glDeleteShader(vertex_shader);
    throw std::exception("Unable to create fragment shader");
  }

  // Geometry
  if (config.geometry) {
    shader_src_path.replace_extension(kGeometryExt);
    geometry_shader = create_shader(read_shader_src(shader_src_path), GL_GEOMETRY_SHADER);
    if (geometry_shader == 0) {
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      throw std::exception("Unable to create geometry shader");
    }
  }

  // Tessellation
  if (config.tessellation) {
    shader_src_path.replace_extension(kTessControlExt);
    tess_control_shader = create_shader(read_shader_src(shader_src_path), GL_TESS_CONTROL_SHADER);
    if (tess_control_shader == 0) {
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      glDeleteShader(geometry_shader);
      throw std::exception("Unable to create tessellation control shader");
    }

    shader_src_path.replace_extension(kTessEvalExt);
    tess_evaluation_shader = create_shader(read_shader_src(shader_src_path), GL_TESS_EVALUATION_SHADER);
    if (tess_evaluation_shader == 0) {
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
  if (config.geometry) {
    glAttachShader(renderer_id_, geometry_shader);
  }

  if (config.tessellation) {
    glAttachShader(renderer_id_, tess_control_shader);
    glAttachShader(renderer_id_, tess_evaluation_shader);
  }

  link_shader();

  glDetachShader(renderer_id_, vertex_shader);
  glDetachShader(renderer_id_, fragment_shader);
  if (config.geometry) {
    glDetachShader(renderer_id_, geometry_shader);
  }
  if (config.tessellation) {
    glDetachShader(renderer_id_, tess_control_shader);
    glDetachShader(renderer_id_, tess_evaluation_shader);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  if (config.geometry) {
    glDeleteShader(geometry_shader);
  }
  if (config.tessellation) {
    glDeleteShader(tess_control_shader);
    glDeleteShader(tess_evaluation_shader);
  }
}

OpenglShader::OpenglShader(std::string_view name, std::string_view vertex_src, std::string_view fragment_src)
    : renderer_id_(0), name_(std::move(name)) {
  create_program(vertex_src, fragment_src);
}

OpenglShader::~OpenglShader() { glDeleteProgram(renderer_id_); }

void OpenglShader::bind() const { glUseProgram(renderer_id_); }

void OpenglShader::unbind() const { glUseProgram(0); }

// <Uniforms>
GLint OpenglShader::get_uniform_location(std::string_view name) const {
  auto it = uniform_locations_.find(name);
  if (it != uniform_locations_.end()) {
    return it->second;
  }

  const std::string key(name);
  const GLint location    = glGetUniformLocation(renderer_id_, key.c_str());
  uniform_locations_[key] = location;
  if (location == -1) {
    KEN_CORE_ERROR("Unable to find uniform '{0}' in shader '{1}'", name, name_);
    return -1;
  }

  KEN_CORE_DEBUG("Adding new uniform location for '{0}' = {1}", name, location);
  return location;
}

void OpenglShader::bind_uniform_buffer(std::string_view name, size_t binding) const {
  GLuint block_index = glGetUniformBlockIndex(renderer_id_, name.data());  // NOLINT
  if (block_index == GL_INVALID_INDEX) {
    KEN_CORE_ERROR("Unable to find uniform block '{0}' in shader '{1}'", name, name_);
    return;
  }

  uniform_block_bindings_[block_index] = static_cast<GLuint>(binding);
  glUniformBlockBinding(renderer_id_, block_index, static_cast<GLuint>(binding));
  KEN_CORE_DEBUG("Adding new uniform block '{0}' (index: {1}) in shader '{2}' to binding: {3}", name, block_index,
                 name_, binding);
}

void OpenglShader::bind_uniform_buffer(std::string_view name, const UniformBuffer& ubo) const {
  GLuint block_index = glGetUniformBlockIndex(renderer_id_, name.data());  // NOLINT
  if (block_index == GL_INVALID_INDEX) {
    KEN_CORE_ERROR("Unable to find uniform block '{0}' in shader '{1}'", name, name_);
    return;
  }

  GLint size = 0;
  glGetActiveUniformBlockiv(renderer_id_, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

  // TODO(kuzu): handle padding
  if (size != static_cast<GLint>(ubo.underlying_buffer()->size())) {
    KEN_CORE_ERROR("Unexpected uniform block size: {0}. Expected: {1}", size, ubo.underlying_buffer()->size());
    return;
  }

  uniform_block_bindings_[block_index] = static_cast<GLuint>(ubo.binding_point());
  glUniformBlockBinding(renderer_id_, block_index, uniform_block_bindings_[block_index]);
  KEN_CORE_DEBUG("Adding new uniform block '{0}' (index: {1}) in shader '{2}' to binding: {3}", name, block_index,
                 name_, ubo.binding_point());
}

// </Uniforms>

#include <shaders.hpp>
const std::unordered_map<std::string_view, std::string_view> OpenglShader::kInternalLibs = GENERATED_SHADER_MAP;

}  // namespace kEn
