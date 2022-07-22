#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec4 vColor;
void main()
{
    vColor = aColor;
    gl_Position = u_Transform * u_ViewProjection * vec4(aPos, 1.0);
}