#include <fstream>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kEn/scene/mesh/obj_model.hpp>
#include <kenpch.hpp>
#include <string>

namespace kEn {

const std::filesystem::path ObjModel::kModelPath("assets/models");

BufferLayout ObjModel::obj_layout_ = {{shader_data_types::float3, "a_Position"},
                                      {shader_data_types::float3, "a_Normal"},
                                      {shader_data_types::float2, "a_TexCoord"}};

ObjModel::ObjModel(const std::filesystem::path& path) {
  vertex_array_ = VertexArray::create();

  std::ifstream file(kModelPath / path);
  std::vector<mEn::Vec3> positions;
  std::vector<mEn::Vec2> texture_coords;
  std::vector<mEn::Vec3> normals;

  std::vector<ObjVertex> final_vertices;
  std::vector<uint32_t> final_indices;

  std::string curline;
  while (std::getline(file, curline)) {
    std::istringstream iss(curline);

    std::string first;
    iss >> first;
    // pos
    if (first == "v") {
      std::string tok;
      mEn::Vec3 pos;
      iss >> tok;
      pos.x = std::stof(tok);
      iss >> tok;
      pos.y = std::stof(tok);
      iss >> tok;
      pos.z = std::stof(tok);

      positions.push_back(pos);
    }

    // normal
    if (first == "vn") {
      std::string tok;
      mEn::Vec3 norm;
      iss >> tok;
      norm.x = std::stof(tok);
      iss >> tok;
      norm.y = std::stof(tok);
      iss >> tok;
      norm.z = std::stof(tok);

      normals.push_back(norm);
    }

    // texture coord
    if (first == "vt") {
      std::string tok;
      mEn::Vec2 tex;
      iss >> tok;
      tex.x = std::stof(tok);
      iss >> tok;
      tex.y = std::stof(tok);

      texture_coords.push_back(tex);
    }

    // face
    if (first == "f") {
      std::string tok;
      std::vector<std::string> v;
      std::vector<ObjVertex> vertices;
      iss >> tok;
      v.push_back(tok);
      iss >> tok;
      v.push_back(tok);
      iss >> tok;
      v.push_back(tok);

      bool normal = false;
      for (size_t i = 0; i < 3; i++) {
        ObjVertex vert{};
        std::vector<std::string> is;
        std::istringstream iss2(v[i]);
        std::string w;

        while (std::getline(iss2, w, '/')) {
          is.push_back(w);
        }

        if (is.size() == 1) {
          vert.pos       = positions[std::stoull(is[0]) - 1];
          vert.tex_coord = mEn::Vec2(0, 0);
          vertices.push_back(vert);
        } else if (is.size() == 2) {
          vert.pos       = positions[std::stoull(is[0]) - 1];
          vert.tex_coord = texture_coords[std::stoull(is[1]) - 1];
          vertices.push_back(vert);
        } else if (is[1].empty()) {
          vert.pos       = positions[std::stoull(is[0]) - 1];
          vert.tex_coord = mEn::Vec2(0, 0);
          vert.normal    = normals[std::stoull(is[2]) - 1];
          normal         = true;
          vertices.push_back(vert);
        } else {
          vert.pos       = positions[std::stoull(is[0]) - 1];
          vert.tex_coord = texture_coords[std::stoull(is[1]) - 1];
          vert.normal    = normals[std::stoull(is[2]) - 1];
          normal         = true;
          vertices.push_back(vert);
        }
      }

      if (!normal) {
        mEn::Vec3 a    = vertices[0].pos - vertices[1].pos;
        mEn::Vec3 b    = vertices[2].pos - vertices[1].pos;
        mEn::Vec3 norm = mEn::cross(a, b);

        for (size_t i = 0; i < 3; i++) {
          vertices[i].normal = norm;
        }
      }

      size_t id0 = final_vertices.size();
      for (size_t i = 0; i < 3; i++) {
        final_vertices.push_back(vertices[i]);
        final_indices.push_back(static_cast<uint32_t>(id0 + i));
      }
    }
  }

  file.close();

  const auto stride   = obj_layout_.stride() / sizeof(float);
  const auto vertices = std::make_unique<float[]>(stride * final_vertices.size());

  size_t offset = 0;
  for (auto vertex : final_vertices) {
    vertices[offset + 0] = vertex.pos.x;
    vertices[offset + 1] = vertex.pos.y;
    vertices[offset + 2] = vertex.pos.z;

    vertices[offset + 3] = vertex.normal.x;
    vertices[offset + 4] = vertex.normal.y;
    vertices[offset + 5] = vertex.normal.z;

    vertices[offset + 6] = vertex.tex_coord.x;
    vertices[offset + 7] = vertex.tex_coord.y;

    offset += stride;
  }

  auto vertex_buffer = Buffer::create(vertices.get(), obj_layout_.stride() * final_vertices.size());
  vertex_buffer->set_layout(obj_layout_);
  auto index_buffer = Buffer::create(final_indices.data(), final_indices.size() * sizeof(uint32_t));

  vertex_array_->add_vertex_buffer(vertex_buffer);
  vertex_array_->set_index_buffer(index_buffer);
}

}  // namespace kEn
