#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include "sprite2d.hpp"

namespace Viper::Renderer {
    Sprite2D::Sprite2D( const std::string& path, int width, int height ) : Path(path), Width( width ), Height( height ) {
        glGenTextures(1, &SpriteID);
        glBindTexture(GL_TEXTURE_2D, SpriteID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int m_TexWidth, m_TexHeight, m_TexChannels;
        unsigned char* m_TextureData = stbi_load(path.c_str( ), &m_TexWidth, &m_TexHeight, NULL, 4);

        if( m_TextureData ) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_TexWidth, m_TexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureData);
            glGenerateMipmap(GL_TEXTURE_2D);
        };

        stbi_image_free(m_TextureData);
        glBindTexture(GL_TEXTURE_2D, 0);
     };
    
    Sprite2D::~Sprite2D() {
        glDeleteTextures(1, &SpriteID);
    };

    Ref< Sprite2D > Sprite2D::Create( const std::string& sprite_path ) {
        return CreateRef< Sprite2D >( sprite_path, 1, 1 );
    };

    Ref< Sprite2D > Sprite2D::Create( const std::string& sprite_path, int width, int height ) {
        return CreateRef< Sprite2D >( sprite_path, width, height );
    };

    uint32_t Sprite2D::GetSprite() const {
        return SpriteID;
    };

    void Sprite2D::Bind() {
        glBindTexture(GL_TEXTURE_2D, SpriteID);
    };

    void Sprite2D::Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    };
};