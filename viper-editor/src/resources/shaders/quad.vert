#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;
layout (location = 4) in float aTexTiling;

uniform mat4 u_ViewProjection;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;
out float vTexTiling;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    vTexTiling = aTexTiling;
    
    gl_Position = u_ViewProjection * vec4(aPos, 1.0);
}