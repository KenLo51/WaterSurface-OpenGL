# version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_color;
layout(location = 3) in vec2 in_texcoord;

uniform mat4 model;
uniform mat4 view;

out vec2 TexCoord;

void main(){
	gl_Position = view * model * vec4(in_position, 1.0);;
	TexCoord = in_texcoord;
}