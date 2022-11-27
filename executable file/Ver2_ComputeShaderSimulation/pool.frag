#version 330 core

in vec3 position_vert;
in vec3 normal_vert;
in vec4 color_vert;
in vec4 proj2lightPos_vert;
in vec4 proj2underwaterPos_vert;
in vec2 texcoord_vert;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool enableCaustics;

uniform sampler2D shadowMap;
uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;
uniform sampler2D underWaterMap;
uniform sampler2D CausticsMap;
void main()
{
    // perform perspective divide
    vec3 proj2lightPos;
    vec3 proj2underwaterPos;
    proj2lightPos = proj2lightPos_vert.xyz / proj2lightPos_vert.w;
    proj2underwaterPos = proj2underwaterPos_vert.xyz / proj2underwaterPos_vert.w;
    // transform from [-1, 1] to [0,1] range
    proj2lightPos = proj2lightPos * 0.5 + 0.5;
    proj2underwaterPos = proj2underwaterPos * 0.5 + 0.5;
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

//    float closestDepth = texture(shadowMap, proj2lightPos.xy).r; 
//    float currDepth = proj2lightPos.z;

    vec3 normal = normalize(normal_vert);
    vec3 position = position_vert;
    //vec4 color = color_vert;
    vec4 color = texture(texDiffuse, texcoord_vert);
    //vec4 color = vec4(texcoord_vert.x, texcoord_vert.y, 0.0f, 1.0f);

    vec3 lightDirect = normalize( lightPos-vec3(0,0,0) ); // object -> light
    vec3 viewDirect = normalize( viewPos-position ); // object -> view

    float ambient = 1.0f;
    float diffuse = max(0.0f, dot(lightDirect, normal));
    //Blinn-Phong specular
    float specular = max(0.0f, dot( normalize(lightDirect+viewDirect) , normal) );
    
    // weights
    ambient = 0.6f * ambient;
    diffuse = 0.4f * diffuse;
    specular = 15.0f * pow(specular, 50) * texture(texSpecular, texcoord_vert).r;


    // Caustics
    const int CausticsRange = 10;
    float CausticsAmp = 0;
    if(enableCaustics){
        if(texture(underWaterMap, proj2underwaterPos.xy).r < (proj2underwaterPos.z+0.001) && shadowDec>0.9){
            if(texture(CausticsMap, proj2lightPos.xy).r < 1.0f){
    //            for(int x=-CausticsRange; x<=CausticsRange; x++){
    //                for(int y=-CausticsRange; y<=CausticsRange; y++){
    //                    CausticsAmp += (1.0f / CausticsRange / CausticsRange) * max(0, dot(texture(CausticsMap, proj2lightPos.xy + vec2( x, y)/1024.0f).xyz, normalize(vec3( x, -position.y, y))) );
    //                }
    //            }
    //        CausticsAmp = 1.0f - CausticsAmp;
    //        CausticsAmp = max(0.1, pow(CausticsAmp, 4) * 2);
                for(int x=-CausticsRange; x<=CausticsRange; x++){
                    for(int y=-CausticsRange; y<=CausticsRange; y++){
                        if(x==0 && y==0) continue;
                        CausticsAmp += (1.0f / CausticsRange / CausticsRange) * 
                            dot(texture(CausticsMap, proj2lightPos.xy + vec2( x, y)/1024.0f).xz, -normalize(vec2( x, y))) * max(0, proj2underwaterPos.z - texture(underWaterMap, proj2underwaterPos.xy).r);
                    }
                }
            }
            CausticsAmp = clamp(CausticsAmp * 10, 0.0, 0.8);
        }
        else{
            CausticsAmp = 0;
        }
    }
    else{
        CausticsAmp = 0;
    }


    float underWaterDec = 0;
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            underWaterDec += (texture(underWaterMap, proj2underwaterPos.xy + vec2(x,y)/1024.0f).r < proj2underwaterPos.z)? 1.0:0.0;
        }
    }
    underWaterDec = underWaterDec / 9;

    FragColor = (ambient + (diffuse + specular) * shadowDec + CausticsAmp) * color;
    FragColor = FragColor - 0.2 * underWaterDec;
} 
