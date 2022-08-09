#version 460 core
layout (location = 0) out vec4 FragColor;
in vec4 vColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vColor;
}