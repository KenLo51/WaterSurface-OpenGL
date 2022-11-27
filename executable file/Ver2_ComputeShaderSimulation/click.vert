# version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 3) in vec2 in_texcoord;

uniform mat4 model;
uniform mat4 view;

out vec2 texcoord_vert;

void main(){
	vec4 position_v4 = model * vec4(in_position, 1.0);
	gl_Position = view * position_v4;
	texcoord_vert = in_texcoord;
}