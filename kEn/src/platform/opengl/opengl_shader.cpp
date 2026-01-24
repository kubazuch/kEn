#include "opengl_shader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/renderer/shader.hpp>

namespace kEn {

namespace {

constexpr const char* shader_stage_name(GLenum type) {
  switch (type) {
    case GL_VERTEX_SHADER:
      return "VERTEX";
    case GL_FRAGMENT_SHADER:
      return "FRAGMENT";
    case GL_GEOMETRY_SHADER:
      return "GEOMETRY";
    case GL_TESS_CONTROL_SHADER:
      return "TESS_CONTROL";
    case GL_TESS_EVALUATION_SHADER:
      return "TESS_EVALUATION";
    case GL_COMPUTE_SHADER:
      return "COMPUTE";
    default:
      return "UNKNOWN";
  }
}

std::filesystem::path normalize_key(const std::filesystem::path& filePath, bool internal) {
  if (internal) {
    return (std::filesystem::path{"__internal__"} / filePath).lexically_normal();
  }

  std::error_code ec;
  auto p = std::filesystem::weakly_canonical(filePath, ec);
  if (ec) {
    p = std::filesystem::absolute(filePath).lexically_normal();
  }

  return p;
}

std::string display_name_for(const std::filesystem::path& key, bool internal) {
  if (internal) {
    return (std::string{"<internal>/"} + key.filename().string());
  }

  // keep logs readable: prefer path relative to kShaderPath when possible
  std::error_code ec;
  auto rel = std::filesystem::relative(key, OpenglShader::kShaderPath, ec);
  if (!ec) {
    const auto dotdot = std::filesystem::path("..");
    if (rel.begin() != rel.end() && *rel.begin() != dotdot) {
      return rel.string();
    }
  }
  return key.string();
}

std::string annotate_glsl_log(std::string_view raw, const std::vector<std::string>& map) {
  std::stringstream in((std::string(raw)));
  std::string line;
  std::ostringstream out;

  std::regex nvidia(R"(^\s*(\d+)\((\d+)\)\s*:\s*(.*)$)");  // 2(15): error ...
  std::regex mesa(R"(^\s*(\d+):(\d+):\s*(.*)$)");          // 2:15: error ...
  while (std::getline(in, line)) {
    std::smatch m;
    if (std::regex_match(line, m, nvidia) || std::regex_match(line, m, mesa)) {
      auto id  = static_cast<size_t>(std::stoul(m[1].str()));
      auto ln  = m[2].str();
      auto msg = m[3].str();
      if (id < map.size()) {
        out << map[id] << ":" << ln << ": " << msg << "\n";
      } else {
        out << line << "\n";
      }
    } else {
      out << line << "\n";
    }
  }
  return out.str();
}

}  // namespace

const std::filesystem::path OpenglShader::kVertexExt(".vert");
const std::filesystem::path OpenglShader::kFragmentExt(".frag");
const std::filesystem::path OpenglShader::kGeometryExt(".geom");
const std::filesystem::path OpenglShader::kTessControlExt(".tesc");
const std::filesystem::path OpenglShader::kTessEvalExt(".tese");

const std::regex OpenglShader::kIncludeRegex(R"(^\s*#\s*include\s+\"(.+)\"\s*$)");
const std::regex OpenglShader::kPragmaOnceRegex(R"(^\s*#\s*pragma\s+once\s*$)");

struct OpenglShader::PreprocessContext {
  std::unordered_set<std::filesystem::path> in_progress;
  std::unordered_set<std::filesystem::path> once_included;
  std::unordered_map<std::filesystem::path, size_t> path_to_id;
  std::vector<std::string> id_to_name;

  size_t get_id(const std::filesystem::path& key, std::string_view display_name) {
    auto [it, inserted] = path_to_id.emplace(key, id_to_name.size());
    if (inserted) {
      id_to_name.emplace_back(display_name);
    }
    return it->second;
  }
};

std::string OpenglShader::read_shader_src_internal(const std::filesystem::path& filePath, PreprocessContext& ctx,
                                                   bool internal) {
  const auto key_path = normalize_key(filePath, internal);
  const auto this_id  = ctx.get_id(key_path, display_name_for(key_path, internal));

  // Detect circular dependencies
  if (ctx.in_progress.contains(key_path)) {
    if (!internal) {
      KEN_CORE_WARN("Circular shader include detected at: {}", display_name_for(key_path, internal));
    }
    return "";
  }

  // #pragma once support
  if (ctx.once_included.contains(key_path)) {
    return "";
  }

  ctx.in_progress.insert(key_path);

  // Load shader from internal libs or read file from disk
  std::unique_ptr<std::istream> stream;
  if (internal) {
    auto key = filePath.string();
    auto it  = kInternalLibs.find(key);
    if (it == kInternalLibs.end()) {
      KEN_CORE_ERROR("Internal shader include not found: {}", key);
      ctx.in_progress.erase(key_path);
      return "";
    }

    stream = std::make_unique<std::stringstream>(std::string(it->second));
  } else {
    auto file = std::make_unique<std::ifstream>(key_path);
    if (!file->is_open()) {
      KEN_CORE_ERROR("Error opening file: {}", key_path.string());
      ctx.in_progress.erase(key_path);
      return "";
    }

    stream = std::move(file);
  }

  // Handle includes
  std::stringstream buffer;
  std::string line;
  size_t line_no       = 0;
  bool saw_pragma_once = false;

  while (std::getline(*stream, line)) {
    ++line_no;

    // Strip accidental #version from included files (keeps root valid)
    if (line_no == 1 && line.starts_with("#version") && this_id != 0) {
      KEN_CORE_WARN("Ignoring #version in included file: {}", display_name_for(key_path, internal));
      continue;
    }

    // Handle #pragma once
    if (std::regex_match(line, kPragmaOnceRegex)) {
      saw_pragma_once = true;
      continue;  // don't emit
    }

    std::smatch include_match;
    if (std::regex_match(line, include_match, kIncludeRegex)) {
      std::string include_file_name = include_match[1].str();
      std::string included_source;

      size_t child_id = 0;

      if (auto it = kInternalLibs.find(include_file_name); it != kInternalLibs.end()) {
        auto child_key  = normalize_key(include_file_name, true);
        child_id        = ctx.get_id(child_key, display_name_for(child_key, true));
        included_source = read_shader_src_internal(include_file_name, ctx, true);
      } else {
        // Resolve filename relative to current file first
        std::filesystem::path candidate = key_path.parent_path() / include_file_name;
        if (!std::filesystem::exists(candidate)) {
          candidate = kShaderPath / include_file_name;
        }
        auto child_key  = normalize_key(candidate, false);
        child_id        = ctx.get_id(child_key, display_name_for(child_key, false));
        included_source = read_shader_src_internal(candidate, ctx, false);
      }

      // #line injection for better debug info
      buffer << "#line 1 " << child_id << "\n";
      buffer << included_source << "\n";
      buffer << "#line " << (line_no + 1) << " " << this_id << "\n";
    } else {
      buffer << line << "\n";
    }
  }

  if (saw_pragma_once) {
    ctx.once_included.insert(key_path);
  }

  ctx.in_progress.erase(key_path);
  return buffer.str();
}

std::string OpenglShader::read_shader_src(const std::filesystem::path& file,
                                          std::vector<std::string>* out_source_id_map) {
  PreprocessContext ctx;
  auto root_key = normalize_key(file, false);
  ctx.get_id(root_key, display_name_for(root_key, false));

  auto src = read_shader_src_internal(file, ctx, false);

  if (out_source_id_map) {
    *out_source_id_map = ctx.id_to_name;
  }

  return src;
}

GLuint OpenglShader::create_shader(std::string_view src, GLenum type, std::string_view debug_name,
                                   std::vector<std::string>* source_id_map) {
  const GLuint shader = glCreateShader(type);
  if (shader == 0) {
    KEN_CORE_ERROR("Shader construction failed! stage={} file={}", shader_stage_name(type), debug_name);
    return 0;
  }

  // Create and compile shader
  const GLchar* source = src.data();
  const auto len       = static_cast<GLint>(src.size());
  glShaderSource(shader, 1, &source, &len);
  glCompileShader(shader);

  // Verify compilation status
  GLint status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> info(static_cast<size_t>(length));

    glGetShaderInfoLog(shader, length, &length, info.data());

    glDeleteShader(shader);

    std::string log = info.data();
    if (source_id_map && !source_id_map->empty()) {
      log = annotate_glsl_log(log, *source_id_map);

      std::ostringstream mapping;
      for (size_t i = 0; i < source_id_map->size(); ++i) {
        mapping << "  " << i << ": " << (*source_id_map)[i] << "\n";
      }
      KEN_CORE_ERROR("GLSL source-id map:\n{}", mapping.str());
    }

    KEN_CORE_ERROR("GLSL compile failed: stage={} file={}\n{}", shader_stage_name(type), debug_name, log);
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

    KEN_CORE_ERROR("GLSL link failed: program='{}'\n{}", name_, info.data());
    KEN_CORE_ASSERT(false, "Shader program linking failed!");
  }

#ifdef _KEN_DEBUG
  glValidateProgram(renderer_id_);

  status = 0;
  glGetProgramiv(renderer_id_, GL_VALIDATE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint length = 0;
    glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> info(static_cast<size_t>(length));
    glGetProgramInfoLog(renderer_id_, length, &length, info.data());

    glDeleteProgram(renderer_id_);

    KEN_CORE_ERROR("GLSL validation failed: program='{}'\n{}", name_, info.data());
    KEN_CORE_ASSERT(false, "Shader program validation failed!");
  }
#endif
}

void OpenglShader::create_program(std::string_view vertex_src, std::string_view fragment_src) {
  const std::string vlabel = name_ + " (vertex)";
  const std::string flabel = name_ + " (fragment)";

  GLuint vertex_shader = create_shader(vertex_src, GL_VERTEX_SHADER, vlabel);
  if (vertex_shader == 0) {
    return;
  }

  GLuint fragment_shader = create_shader(fragment_src, GL_FRAGMENT_SHADER, flabel);
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

  std::vector<std::string> map;

  // Vertex
  shader_src_path.replace_extension(kVertexExt);
  auto src      = read_shader_src(shader_src_path, &map);
  vertex_shader = create_shader(src, GL_VERTEX_SHADER, shader_src_path.string(), &map);
  if (vertex_shader == 0) {
    throw std::runtime_error("Unable to create vertex shader");
  }

  // Fragment
  shader_src_path.replace_extension(kFragmentExt);
  src             = read_shader_src(shader_src_path, &map);
  fragment_shader = create_shader(src, GL_FRAGMENT_SHADER, shader_src_path.string(), &map);
  if (fragment_shader == 0) {
    glDeleteShader(vertex_shader);
    throw std::runtime_error("Unable to create fragment shader");
  }

  // Geometry
  if (config.geometry) {
    shader_src_path.replace_extension(kGeometryExt);
    src             = read_shader_src(shader_src_path, &map);
    geometry_shader = create_shader(src, GL_GEOMETRY_SHADER, shader_src_path.string(), &map);
    if (geometry_shader == 0) {
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      throw std::runtime_error("Unable to create geometry shader");
    }
  }

  // Tessellation
  if (config.tessellation) {
    shader_src_path.replace_extension(kTessControlExt);
    src                 = read_shader_src(shader_src_path, &map);
    tess_control_shader = create_shader(src, GL_TESS_CONTROL_SHADER, shader_src_path.string(), &map);
    if (tess_control_shader == 0) {
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      glDeleteShader(geometry_shader);
      throw std::runtime_error("Unable to create tessellation control shader");
    }

    shader_src_path.replace_extension(kTessEvalExt);
    src                    = read_shader_src(shader_src_path, &map);
    tess_evaluation_shader = create_shader(src, GL_TESS_EVALUATION_SHADER, shader_src_path.string(), &map);
    if (tess_evaluation_shader == 0) {
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      glDeleteShader(geometry_shader);
      glDeleteShader(tess_control_shader);
      throw std::runtime_error("Unable to create tessellation evaluation shader");
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
    KEN_CORE_WARN("Unable to find uniform '{0}' in shader '{1}'", name, name_);
    return -1;
  }

  KEN_CORE_DEBUG("Adding new uniform location for '{0}' = {1}", name, location);
  return location;
}

void OpenglShader::bind_uniform_buffer(std::string_view name, size_t binding) const {
  std::string tmp(name);
  GLuint block_index = glGetUniformBlockIndex(renderer_id_, tmp.c_str());  // NOLINT
  if (block_index == GL_INVALID_INDEX) {
    KEN_CORE_WARN("Unable to find uniform block '{0}' in shader '{1}'", name, name_);
    return;
  }

  uniform_block_bindings_[block_index] = static_cast<GLuint>(binding);
  glUniformBlockBinding(renderer_id_, block_index, static_cast<GLuint>(binding));
  KEN_CORE_DEBUG("Adding new uniform block '{0}' (index: {1}) in shader '{2}' to binding: {3}", name, block_index,
                 name_, binding);
}

void OpenglShader::bind_uniform_buffer(std::string_view name, const UniformBuffer& ubo) const {
  std::string tmp(name);
  GLuint block_index = glGetUniformBlockIndex(renderer_id_, tmp.c_str());  // NOLINT
  if (block_index == GL_INVALID_INDEX) {
    KEN_CORE_WARN("Unable to find uniform block '{0}' in shader '{1}'", name, name_);
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
