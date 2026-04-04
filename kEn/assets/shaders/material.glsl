#pragma once

const int MAX_TEXTURES = 5;

struct material {
  float ka;
  float kd;
  float ks;
  float m;

  vec3 surface_color;

  bool emissive;

  int diffuse_count;
  int height_count;
  int normal_count;
  int specular_count;
};
