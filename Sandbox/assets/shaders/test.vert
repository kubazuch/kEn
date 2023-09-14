#version 330 core
				
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
				
uniform mat4 u_M;
uniform mat4 u_VP;

out vec3 v_Pos;

void main() {
	v_Pos = a_Position;
	gl_Position = u_VP * u_M * vec4(a_Position, 1.0);
}