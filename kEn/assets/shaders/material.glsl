const int MAX_TEXTURES = 5;

struct material {
  float ka;
  float kd;
  float ks;
  float m;

  bool emissive;

  int diffuse_count;
  sampler2D diffuse[MAX_TEXTURES];
};