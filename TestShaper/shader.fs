#version 330 core

in vec3 Color;
in vec2 TexCoord;

uniform sampler2D Texture;

out vec4 FragColor;

void main()
{
    float texColorRed = texture(Texture, TexCoord).r;
    FragColor = vec4(texColorRed, texColorRed, texColorRed, 1.0);
}
