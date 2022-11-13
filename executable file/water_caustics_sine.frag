#version 330 core

in vec3 position_vert;
in vec3 normal_vert;
in vec2 texcoord_vert;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec2 theta;
uniform float ampltidue;
uniform vec2 texOffset;
uniform vec2 texScale;

void main()
{

    //vec3 normal = normalize(normal_vert);
    //vec3 normal = texture(texNormal, texcoord_vert).rbg * 2 - 1;
    vec3 normal;
    normal.y = 1.0;

    //position_v4.y = position_v4.y + (sin(theta * texScale.x + texOffset.x) + sin(theta * texScale.y + texOffset.y)) * ampltidue;
    normal.x =  -cos((texcoord_vert.x + texcoord_vert.y)) * texScale.x;
    normal.z =  cos((texcoord_vert.x + texcoord_vert.y)) * texScale.y;
    normal.xz = normal.xz * ampltidue * 0.1;
    normal = normalize(normal);

    // combine all
    FragColor = vec4(normal, 1.0f);
} 
