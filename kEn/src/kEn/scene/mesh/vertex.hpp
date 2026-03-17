#pragma once

#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>

#include <kEn/core/core.hpp>

namespace kEn {

struct Vertex {
  mEn::Vec3 pos;
  mEn::Vec3 normal;
  mEn::Vec2 texture_coord;
};

}  // namespace kEn
