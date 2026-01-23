#pragma once

const int MAX_TEXTURES = 5;

struct material {
  float ka;
  float kd;
  float ks;
  float m;

  vec3 surface_color;

  float sigma;
  vec3 sss_color;
  float sss_strength;

  bool emissive;

  int diffuse_count;
  sampler2D diffuse[MAX_TEXTURES];
  int normal_count;
  sampler2D normal[MAX_TEXTURES];
};