#version 330 core

layout (location = 0) in vec3 attrPosition;
layout (location = 1) in vec3 attrColor;
layout (location = 2) in vec2 attrTexCoord;

out vec3 Color;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(attrPosition, 1.0);
    Color = attrColor;
    TexCoord = vec2(attrTexCoord.x, attrTexCoord.y);
}
