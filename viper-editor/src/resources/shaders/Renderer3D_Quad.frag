#version 460 core
layout (location = 0) out vec4 FragColor;

in flat vec4 vColor;
in vec3 vNormals;
in vec3 vPos;

void main()
{
    FragColor = vColor;
}