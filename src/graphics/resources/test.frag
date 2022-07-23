#version 410 core
layout (location = 0) out vec4 FragColor;

uniform vec2 u_LightPos;
uniform float u_LightDensity;

in vec4 vColor;
in vec4 pos;

void main()
{
    float intensity = 1.0 / length(pos.xy - u_LightPos) * u_LightDensity;
    FragColor = vColor * intensity;
}