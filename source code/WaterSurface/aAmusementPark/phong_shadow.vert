# version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_color;
layout(location = 3) in vec2 in_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 light;

out vec3 position_vert;
out vec3 normal_vert;
out vec4 color_vert;
out vec4 proj2lightPos_vert;
out vec2 texcoord_vert;

void main(){
	vec4 position_v4 = model * vec4(in_position, 1.0);
	gl_Position = view * position_v4;

	texcoord_vert = in_texcoord;
	position_vert = position_v4.xyz;
	normal_vert = vec3(model * vec4(in_normal, 1.0f));
	color_vert = in_color;
	proj2lightPos_vert = light * position_v4;
}