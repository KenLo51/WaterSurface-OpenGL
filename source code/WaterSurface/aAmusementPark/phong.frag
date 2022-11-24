#version 330 core

in vec3 position_vert;
in vec3 normal_vert;
in vec4 color_vert;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 normal = normalize(normal_vert);
    vec3 position = position_vert;
    vec4 color = color_vert;

    vec3 lightDirect = normalize( lightPos-vec3(0,0,0) ); // object -> light
    vec3 viewDirect = normalize( viewPos-position ); // object -> view

    float ambient = 1.0f;
    float diffuse = max(0.0f, dot(lightDirect, normal));
    //Blinn-Phong specular
    float specular = max(0.0f, dot( (lightDirect+lightDirect)/length(lightDirect+lightDirect) , normal) );
    
    // weights
    ambient = 0.1f * ambient;
    diffuse = 0.9f * diffuse;
    specular = 0.7f * pow(specular, 20);

    // combine all
    FragColor = (ambient + diffuse + specular) * color;
} 
