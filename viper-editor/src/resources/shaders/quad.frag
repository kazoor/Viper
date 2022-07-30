#version 410 core
layout (location = 0) out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;

uniform sampler2D u_Textures[32];

void main()
{
    if( vTexIndex > 0 ) {
        FragColor = texture(u_Textures[int(vTexIndex)], vTexCoord);
    } else {
        FragColor = vColor;
    }
}