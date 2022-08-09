#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 u_ViewProjection;

out vec4 vColor;
out vec3 vPos;

void main()
{
    vColor = aColor;
    vPos = aPos;
    gl_Position = u_ViewProjection * vec4(aPos, 1.0);
}