#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormals;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

out flat vec4 vColor;
out vec3 vNormals;
out vec3 vPos;

void main()
{
    vColor = aColor;
    vNormals = aNormals;
    vPos = aPos;

    gl_Position = u_ViewProjection * vec4(aPos, 1.0);
}