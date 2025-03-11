#pragma once
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <mEn.hpp>


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
  mEn::Vec3 pos;
  mEn::Vec2 tex_coord;
  mEn::Vec3 normal;
};

}  // namespace kEn
