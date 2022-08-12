#version 460 core
layout (location = 0) out vec4 FragColor;

in flat vec4 vColor;

void main()
{
    FragColor = vColor;
}