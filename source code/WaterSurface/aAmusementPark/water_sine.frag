#version 330 core

in vec3 position_vert;
in vec3 normal_vert;
in vec4 color_vert;
in vec4 proj2lightPos_vert;
in vec4 proj2viewPos_vert;
in vec4 proj2Refraction_vert;
in vec2 texcoord_vert;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec2 theta;
uniform float ampltidue;
uniform vec2 texOffset;
uniform vec2 texScale;

uniform sampler2D shadowMap;

uniform sampler2D texRefraction;
uniform sampler2D texReflection;

void main()
{
    // perform perspective divide
    vec3 proj2lightPos;
    vec3 proj2viewPos;
    vec3 proj2Refraction;
    proj2lightPos = proj2lightPos_vert.xyz / proj2lightPos_vert.w;
    proj2viewPos = proj2viewPos_vert.xyz / proj2viewPos_vert.w;
    proj2Refraction = proj2Refraction_vert.xyz / proj2Refraction_vert.w;
    // transform from [-1, 1] to [0,1] range
    proj2lightPos = proj2lightPos * 0.5 + 0.5;
    proj2viewPos = proj2viewPos * 0.5 + 0.5;
    proj2Refraction = proj2Refraction * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float shadowDec = 0;
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float closestDepth = texture(shadowMap, proj2lightPos.xy + vec2(x,y)/1024).r; 
            float currDepth = proj2lightPos.z;
            shadowDec += (currDepth<=closestDepth)? 1.0:0.0;
        }
    }
    shadowDec = shadowDec / 9;

    //vec3 normal = normalize(normal_vert);
    //vec3 normal = texture(texNormal, texcoord_vert).rbg * 2 - 1;
    vec3 normal;
    normal.y = 1.0;

    //position_v4.y = position_v4.y + (sin(theta * texScale.x + texOffset.x) + sin(theta * texScale.y + texOffset.y)) * ampltidue;
    normal.x =  -cos((texcoord_vert.x + texcoord_vert.y)) * texScale.x;
    normal.z =  cos((texcoord_vert.x + texcoord_vert.y)) * texScale.y;
    normal.xz = normal.xz * ampltidue * 0.1;
    normal = normalize(normal);

    vec3 position = position_vert;
    vec3 lightDirect = normalize( lightPos-vec3(0,0,0) ); // object -> light
    vec3 viewDirect = normalize( viewPos-position ); // object -> view

    if(viewPos.y < position.y)
        normal = -normal;

    //vec4 color = texture(texHeight, texcoord_vert).r * vec4(0.8, 0.8, 0.8, 1.0f);
    //vec4 color = vec4(normal, 1.0f);
    //vec4 color = vec4(0.9, 0.9, 0.9, 1.0f);
    vec3 colorRefraction = texture(texRefraction, proj2Refraction.xy + 0.05 * normal.xz).xyz;
    vec3 colorReflection = texture(texReflection, vec2(1-proj2viewPos.x, proj2viewPos.y) + 0.05 * normal.xz).xyz;
    float colorRefraction_t = dot(normal, viewDirect);
    vec4 color = vec4(0.7 * colorRefraction_t * colorRefraction + (1-colorRefraction_t) * colorReflection, 1.0);

    float ambient = 1.0f;
    float diffuse = max(0.0f, dot(lightDirect, normal));
    //Blinn-Phong specular
    float specular = max(0.0f, dot( normalize(lightDirect+viewDirect) , normal) );
    
    // weights
    ambient = 0.6f * ambient;
    diffuse = 0.4f * diffuse;
    specular = 1.0f * pow(specular, 100);

    // combine all
    FragColor = (ambient + (diffuse + specular) * shadowDec) * color;

    FragColor[3] = 1.0f;
} 
