# version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

uniform mat4 model;
uniform mat4 view;

out vec4 position_vert;

void main(){
	vec4 position_v4 = model * vec4(in_position, 1.0);
	gl_Position = view * position_v4;
	position_vert = gl_Position;
	//pos = gl_Position;
}