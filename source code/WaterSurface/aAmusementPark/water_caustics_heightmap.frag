#version 330 core

in vec3 position_vert;
in vec3 normal_vert;
in vec2 texcoord_vert;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ampltidue;

uniform sampler2D texHeight;


void main()
{

    //vec3 normal = normalize(normal_vert);
    //vec3 normal = texture(texNormal, texcoord_vert).rbg * 2 - 1;
    vec3 normal;
    normal.y = 1.0;
    normal.x = -(texture(texHeight, texcoord_vert+vec2(1.0/1024.0, 0.0)).r - texture(texHeight, texcoord_vert-vec2(1.0/1024.0, 0.0)).r);
    normal.z =  (texture(texHeight, texcoord_vert+vec2(0.0, 1.0/1024.0)).r - texture(texHeight, texcoord_vert-vec2(0.0, 1.0/1024.0)).r);
    normal.xz = normal.xz * ampltidue * 10;
    normal = normalize(normal);

    // combine all
    FragColor = vec4(normal, 1.0f);

} 
