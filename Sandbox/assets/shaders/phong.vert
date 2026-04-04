#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

uniform mat4 u_M;
uniform mat4 u_VP;
uniform mat4 u_LightVP;

out vec3 v_FragPos;
out mat3 v_TBN;
out vec2 v_TexCoord;
out vec4 v_FragPosLightSpace;

void main() {
  vec4 world_pos = u_M * vec4(a_Position, 1.0);
  v_FragPos      = world_pos.xyz;
  v_TexCoord     = a_TexCoord;

  mat3 normal_mat = mat3(transpose(inverse(u_M)));

  vec3 T = normalize(normal_mat * a_Tangent.xyz);
  vec3 N = normalize(normal_mat * a_Normal);
  vec3 B = cross(N, T) * a_Tangent.w;
  v_TBN  = mat3(T, B, N);

  v_FragPosLightSpace = u_LightVP * world_pos;
  gl_Position         = u_VP * world_pos;
}
