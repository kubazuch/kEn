#pragma once
#include <glm/glm.hpp>

namespace kEn {

struct Vertex {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 texture_coord;
};

}  // namespace kEn
