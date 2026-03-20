#version 330 core

#include "light"

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform vec3 u_CameraPos;
uniform vec3 u_Ambient;
uniform material u_Material;

out vec4 frag_color;

void main() {
  vec3 norm     = normalize(v_Normal);
  vec3 view_dir = normalize(u_CameraPos - v_FragPos);

  vec3 diff_tex = u_Material.diffuse_count > 0
                    ? texture(u_Material.diffuse[0], v_TexCoord).rgb
                    : u_Material.surface_color;

  vec3 lighting = vec3(0.0);

  for (int i = 0; i < u_DirectionalCount; ++i)
    lighting += calc_dir_light(u_DirectionalLights[i], u_Material, norm, view_dir);

  for (int i = 0; i < u_PointCount; ++i)
    lighting += calc_point_light(u_PointLights[i], u_Material, norm, v_FragPos, view_dir);

  for (int i = 0; i < u_SpotCount; ++i)
    lighting += calc_spot_light(u_SpotLights[i], u_Material, norm, v_FragPos, view_dir);

  vec3 ambient = u_Ambient * u_Material.ka * diff_tex;

  frag_color = vec4(ambient + lighting * diff_tex, 1.0);
}
