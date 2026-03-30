#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_M;
uniform mat4 u_VP;

void main() { gl_Position = u_VP * u_M * vec4(a_Position, 1.0); }
