#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormals;

uniform mat4 viewmatrix;

out flat vec4 vColor;
out vec3 vNormals;

void main()
{
    vColor = aColor;
    vNormals = aNormals;
    gl_Position = viewmatrix * vec4(aPos, 1.0);
}