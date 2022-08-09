#version 460 core
layout (location = 0) out vec4 FragColor;

in vec4 vColor;
in vec3 vPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float u_Time;

void main()
{
    vec3 pos = vPos;
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    pos.x += sin(u_Time) + cos(u_Time * 2.1);
    pos.y += cos(u_Time) + sin(u_Time * 1.6);
    vec3 result = sin( u_Time ) + 0.5 / length( pos ) * ambient * objectColor;
    FragColor = vec4(result, 1.0);//vColor;
}