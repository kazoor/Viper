#version 460 core
layout (location = 0) out vec4 FragColor;

in vec4 vColor;
in vec2 vUV;

uniform sampler2D u_Texture[10];
void main()
{
    vec4 color = texture(u_Texture[0], vUV);
    vec4 next_color = texture(u_Texture[1], vUV) * 0.2;
    vec4 result_color = 1.0 / length(vUV) * color / next_color;
    FragColor = next_color;//vColor;
}