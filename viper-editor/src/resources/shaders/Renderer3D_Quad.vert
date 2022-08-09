#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 viewmatrix;
out vec4 vColor;

void main()
{
    vColor = aColor;
    gl_Position = viewmatrix * vec4(aPos, 1.0);
}