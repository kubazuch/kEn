#include <fstream>
#include <kEn/scene/mesh/obj_model.hpp>
#include <kenpch.hpp>
#include <string>

namespace kEn {

const std::filesystem::path obj_model::model_path("assets/models");

buffer_layout obj_model::obj_layout = {{shader_data_types::float3, "a_Position"},
                                       {shader_data_types::float3, "a_Normal"},
                                       {shader_data_types::float2, "a_TexCoord"}};

obj_model::obj_model(const std::filesystem::path& path) {
  vertex_array_ = vertex_array::create();

  std::ifstream file(model_path / path);
  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> texture_coords;
  std::vector<glm::vec3> normals;

  std::vector<obj_vertex> final_vertices;
  std::vector<unsigned int> final_indices;

  std::string curline;
  while (std::getline(file, curline)) {
    std::istringstream iss(curline);

    std::string first;
    iss >> first;
    // pos
    if (first == "v") {
      std::string tok;
      glm::vec3 pos;
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
      glm::vec3 norm;
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
      glm::vec3 tex;
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
      std::vector<obj_vertex> vertices;
      iss >> tok;
      v.push_back(tok);
      iss >> tok;
      v.push_back(tok);
      iss >> tok;
      v.push_back(tok);

      bool normal = false;
      for (int i = 0; i < 3; i++) {
        obj_vertex vert;
        std::vector<std::string> is;
        std::istringstream iss2(v[i]);
        std::string w;

        while (std::getline(iss2, w, '/')) {
          is.push_back(w);
        }

        if (is.size() == 1) {
          vert.pos       = positions[std::stoi(is[0]) - 1];
          vert.tex_coord = glm::vec2(0, 0);
          vertices.push_back(vert);
        } else if (is.size() == 2) {
          vert.pos       = positions[std::stoi(is[0]) - 1];
          vert.tex_coord = texture_coords[std::stoi(is[1]) - 1];
          vertices.push_back(vert);
        } else if (is[1].empty()) {
          vert.pos       = positions[std::stoi(is[0]) - 1];
          vert.tex_coord = glm::vec2(0, 0);
          vert.normal    = normals[std::stoi(is[2]) - 1];
          normal         = true;
          vertices.push_back(vert);
        } else {
          vert.pos       = positions[std::stoi(is[0]) - 1];
          vert.tex_coord = texture_coords[std::stoi(is[1]) - 1];
          vert.normal    = normals[std::stoi(is[2]) - 1];
          normal         = true;
          vertices.push_back(vert);
        }
      }

      if (!normal) {
        glm::vec3 A    = vertices[0].pos - vertices[1].pos;
        glm::vec3 B    = vertices[2].pos - vertices[1].pos;
        glm::vec3 norm = glm::cross(A, B);

        for (int i = 0; i < 3; i++) {
          vertices[i].normal = norm;
        }
      }

      int id0 = final_vertices.size();
      for (int i = 0; i < 3; i++) {
        final_vertices.push_back(vertices[i]);

        unsigned int ind = id0 + i;
        final_indices.push_back(ind);
      }
    }
  }

  file.close();

  const int stride    = obj_layout.stride() / sizeof(float);
  const auto vertices = new float[stride * final_vertices.size()];

  int offset = 0;
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

  auto vertex_buffer = vertex_buffer::create(vertices, obj_layout.stride() * final_vertices.size());
  vertex_buffer->set_layout(obj_layout);
  auto index_buffer = index_buffer::create(final_indices.data(), final_indices.size());

  vertex_array_->add_vertex_buffer(vertex_buffer);
  vertex_array_->set_index_buffer(index_buffer);

  delete[] vertices;
}

}  // namespace kEn
