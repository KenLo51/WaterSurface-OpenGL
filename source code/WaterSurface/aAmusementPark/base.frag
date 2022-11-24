#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = texture(ourTexture, TexCoord) * 1.0f;
    FragColor = vec4(TexCoord, 0.0f, 1.0f);
} 
