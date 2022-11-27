#version 330 core

in vec4 position_vert;
out vec4 FragColor;
void main()
{
	//FragColor = vec4(1.0f * gl_FragCoord.z * vec3(1.0f, 1.0f, 1.0f), 1.0f);
	FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	//if(gl_FragCoord.z > 0.5) FragColor = vec4(1, 0, 0, 1);
} 
