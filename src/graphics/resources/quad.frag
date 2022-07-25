#version 410 core
layout (location = 0) out vec4 FragColor;

in vec4 vColor;

void main()
{
    FragColor = vColor;
}