#version 330 core

out vec4 FragColor;

in vec2 texcoord_vert;

void main()
{
	FragColor = vec4(texcoord_vert.x, texcoord_vert.y, 1.0, 1.0);
} 
