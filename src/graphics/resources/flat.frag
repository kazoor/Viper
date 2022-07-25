#version 410 core
layout (location = 0) out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;

uniform sampler2D u_Texture;

void main()
{
    FragColor = vColor;
}