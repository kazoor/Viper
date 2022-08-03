#version 460 core
layout (location = 0) out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in float vTexTiling;

uniform sampler2D u_Textures[32];
vec4 vTextureColor;

void main()
{
    vTextureColor = vColor;
    switch(int(vTexIndex)) {
        case 0: vTextureColor *= texture(u_Textures[0], vTexCoord * vTexTiling); break;
        case 1: vTextureColor *= texture(u_Textures[1], vTexCoord * vTexTiling); break;
        case 2: vTextureColor *= texture(u_Textures[2], vTexCoord * vTexTiling); break;
        case 3: vTextureColor *= texture(u_Textures[3], vTexCoord * vTexTiling); break;
        case 4: vTextureColor *= texture(u_Textures[4], vTexCoord * vTexTiling); break;
        case 5: vTextureColor *= texture(u_Textures[5], vTexCoord * vTexTiling); break;
        case 6: vTextureColor *= texture(u_Textures[6], vTexCoord * vTexTiling); break;
        case 7: vTextureColor *= texture(u_Textures[7], vTexCoord * vTexTiling); break;
        case 8: vTextureColor *= texture(u_Textures[8], vTexCoord * vTexTiling); break;
        case 9: vTextureColor *= texture(u_Textures[9], vTexCoord * vTexTiling); break;
        case 10: vTextureColor *= texture(u_Textures[10], vTexCoord * vTexTiling); break;
        case 11: vTextureColor *= texture(u_Textures[11], vTexCoord * vTexTiling); break;
        case 12: vTextureColor *= texture(u_Textures[12], vTexCoord * vTexTiling); break;
        case 13: vTextureColor *= texture(u_Textures[13], vTexCoord * vTexTiling); break;
        case 14: vTextureColor *= texture(u_Textures[14], vTexCoord * vTexTiling); break;
        case 15: vTextureColor *= texture(u_Textures[15], vTexCoord * vTexTiling); break;
        case 16: vTextureColor *= texture(u_Textures[16], vTexCoord * vTexTiling); break;
        case 17: vTextureColor *= texture(u_Textures[17], vTexCoord * vTexTiling); break;
        case 18: vTextureColor *= texture(u_Textures[18], vTexCoord * vTexTiling); break;
        case 19: vTextureColor *= texture(u_Textures[19], vTexCoord * vTexTiling); break;
        case 20: vTextureColor *= texture(u_Textures[20], vTexCoord * vTexTiling); break;
        case 21: vTextureColor *= texture(u_Textures[21], vTexCoord * vTexTiling); break;
        case 22: vTextureColor *= texture(u_Textures[22], vTexCoord * vTexTiling); break;
        case 23: vTextureColor *= texture(u_Textures[23], vTexCoord * vTexTiling); break;
        case 24: vTextureColor *= texture(u_Textures[24], vTexCoord * vTexTiling); break;
        case 25: vTextureColor *= texture(u_Textures[25], vTexCoord * vTexTiling); break;
        case 26: vTextureColor *= texture(u_Textures[26], vTexCoord * vTexTiling); break;
        case 27: vTextureColor *= texture(u_Textures[27], vTexCoord * vTexTiling); break;
        case 28: vTextureColor *= texture(u_Textures[28], vTexCoord * vTexTiling); break;
        case 29: vTextureColor *= texture(u_Textures[29], vTexCoord * vTexTiling); break;
        case 30: vTextureColor *= texture(u_Textures[30], vTexCoord * vTexTiling); break;
        case 31: vTextureColor *= texture(u_Textures[31], vTexCoord * vTexTiling); break;
    }

    if( vTextureColor.a == 0.0 )
        discard;

    FragColor = vTextureColor;
}