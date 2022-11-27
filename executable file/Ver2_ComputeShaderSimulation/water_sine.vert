# version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_color;
layout(location = 3) in vec2 in_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 light;
uniform mat4 refractionView;

uniform vec3 viewPos;

uniform vec2 theta;
uniform float ampltidue;
uniform vec2 texOffset;
uniform vec2 texScale;

out vec3 position_vert;
out vec3 normal_vert;
out vec4 color_vert;
out vec4 proj2lightPos_vert;
out vec4 proj2viewPos_vert;
out vec4 proj2Refraction_vert;
out vec2 texcoord_vert;

void main(){
	vec4 position_v4 = vec4(in_position, 1.0);
	texcoord_vert = (in_texcoord + texOffset + theta) * texScale * 10.0f;
	position_v4.y = position_v4.y + sin(texcoord_vert.x + texcoord_vert.y) * ampltidue;
	position_v4 = model * position_v4;
	proj2viewPos_vert = view * position_v4;
	gl_Position = proj2viewPos_vert;

	if( viewPos.y > 2.0f)
		proj2Refraction_vert = refractionView * (position_v4 + vec4(0, -0.5, 0, 0));
	else
		proj2Refraction_vert = refractionView * position_v4;

	position_vert = position_v4.xyz;
	normal_vert = vec3(model * vec4(in_normal, 1.0f));
	color_vert = in_color;
	proj2lightPos_vert = light * position_v4;
}