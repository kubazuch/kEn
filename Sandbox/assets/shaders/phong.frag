#version 330 core

#include "light"

in vec3 v_FragPos;
in mat3 v_TBN;
in vec2 v_TexCoord;
in vec4 v_FragPosLightSpace;

uniform vec3 u_CameraPos;
uniform vec3 u_Ambient;

uniform material u_Material;
uniform sampler2D u_Material_diffuse[MAX_TEXTURES];
uniform sampler2D u_Material_normal[MAX_TEXTURES];
uniform sampler2D u_Material_specular[MAX_TEXTURES];

uniform sampler2D u_ShadowMap;
uniform float u_ShadowBias;
uniform bool u_ShadowsEnabled;
uniform bool u_UseNormalMap   = true;
uniform bool u_UseSpecularMap = true;

out vec4 frag_color;

float shadow_factor(vec4 frag_pos_light_space) {
  if (!u_ShadowsEnabled) return 1.0;
  vec3 proj = frag_pos_light_space.xyz / frag_pos_light_space.w;
  proj      = proj * 0.5 + 0.5;
  if (proj.z > 1.0) return 1.0;
  vec2 texel   = 1.0 / textureSize(u_ShadowMap, 0);
  float shadow = 0.0;
  for (int x = -1; x <= 1; ++x)
    for (int y = -1; y <= 1; ++y)
      shadow += (proj.z - u_ShadowBias) > texture(u_ShadowMap, proj.xy + vec2(x, y) * texel).r ? 1.0 : 0.0;
  return 1.0 - shadow / 9.0;
}

void main() {
  // Normal
  vec3 norm;
  if (u_UseNormalMap && u_Material.normal_count > 0) {
    vec3 n = texture(u_Material_normal[0], v_TexCoord).rgb * 2.0 - 1.0;
    norm   = normalize(v_TBN * n);
  } else {
    norm = normalize(v_TBN[2]);
  }

  vec3 view_dir = normalize(u_CameraPos - v_FragPos);

  // Diffuse color
  vec3 diff_tex =
      u_Material.diffuse_count > 0 ? texture(u_Material_diffuse[0], v_TexCoord).rgb : u_Material.surface_color;

  // Specular factor from specular map
  float spec_factor =
      (u_UseSpecularMap && u_Material.specular_count > 0) ? texture(u_Material_specular[0], v_TexCoord).r : 1.0;

  // Scale material ks by specular map
  material mat = u_Material;
  mat.ks *= spec_factor;

  vec3 lighting = vec3(0.0);

  float shadow = shadow_factor(v_FragPosLightSpace);
  for (int i = 0; i < u_DirectionalCount; ++i)
    lighting += shadow * calc_dir_light(u_DirectionalLights[i], mat, norm, view_dir);

  for (int i = 0; i < u_PointCount; ++i) lighting += calc_point_light(u_PointLights[i], mat, norm, v_FragPos, view_dir);

  for (int i = 0; i < u_SpotCount; ++i) lighting += calc_spot_light(u_SpotLights[i], mat, norm, v_FragPos, view_dir);

  vec3 ambient = u_Ambient * mat.ka * diff_tex;

  frag_color = vec4(ambient + lighting * diff_tex, 1.0);
}
