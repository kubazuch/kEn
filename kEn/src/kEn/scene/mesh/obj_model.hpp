#pragma once
#include <glm/glm.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/vertex_array.hpp>

namespace kEn {

class ObjModel {
 public:
  [[deprecated("obj_model is deprecated! Please use model")]]
  explicit ObjModel(const std::filesystem::path& path);

  std::shared_ptr<VertexArray> vertex_array_;

 public:
  static BufferLayout obj_layout_;
  static const std::filesystem::path kModelPath;
};

struct ObjVertex {
  glm::vec3 pos;
  glm::vec2 tex_coord;
  glm::vec3 normal;
};

}  // namespace kEn
