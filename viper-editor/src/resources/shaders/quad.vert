#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
}