#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <util/globals/global.hpp>
#include <viper/base.hpp>
#include "sprite2d.hpp"

namespace Viper {
    Sprite2D::Sprite2D() {
        SpriteID = 0;
        Path = "";
    };

    Sprite2D::Sprite2D( const std::string& path ) : Path(path) {
        int m_texture_width, m_texture_height;
        unsigned char* m_TextureData = stbi_load(path.c_str( ), &m_texture_width, &m_texture_height, NULL, 4);
        if( m_TextureData )
            Globals::ConsoleContext::AddLog( VIPER_ICON_SUCC " Loaded texture.", VIPER_FORMAT_STRING( "Loaded texture: %s, dimensions: %dx%d", path.c_str( ), m_texture_width, m_texture_height ), Globals::ConsoleFlag::ConsoleSuccess );
        else
            Globals::ConsoleContext::AddLog( VIPER_ICON_ERR " Unable to load texture.", VIPER_FORMAT_STRING( "Unable to locate: %s, no such file or directory.", path.c_str( ) ), Globals::ConsoleFlag::ConsoleError );

        if( m_TextureData ) {
            Width = m_texture_width;
            Height = m_texture_height;

            glCreateTextures(GL_TEXTURE_2D, 1, &SpriteID);
            glBindTexture(GL_TEXTURE_2D, SpriteID);

            glTextureStorage2D(SpriteID, 1, GL_RGBA8, Width, Height );

            glTextureParameteri(SpriteID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(SpriteID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(SpriteID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(SpriteID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D(SpriteID, 0, 0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureData);

            stbi_image_free(m_TextureData);
            glBindTexture(GL_TEXTURE_2D, 0);
        };
     };

    Sprite2D::Sprite2D( int width, int height ) {
        Width = width;
        Height = height;

        glCreateTextures(GL_TEXTURE_2D, 1, &SpriteID);
        glBindTexture(GL_TEXTURE_2D, SpriteID);

        glTextureStorage2D(SpriteID, 1, GL_RGBA8, width, height );

        glTextureParameteri(SpriteID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(SpriteID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(SpriteID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(SpriteID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
    };
    
    Sprite2D::~Sprite2D() {
        Delete();
    };

    void Sprite2D::Delete() {
        glDeleteTextures(1, &SpriteID);
    };

    Ref< Sprite2D > Sprite2D::Create( const std::string& sprite_path ) {
        return CreateRef< Sprite2D >( sprite_path );
    };

    Ref< Sprite2D > Sprite2D::Create( int width, int height ) {
        return CreateRef< Sprite2D >( width, height );
    };

    uint32_t Sprite2D::GetSprite() const {
        return SpriteID;
    };

    void Sprite2D::Bind() {
        glBindTexture(GL_TEXTURE_2D, SpriteID);
    };

    void Sprite2D::SetData( void* data ) {
        glTextureSubImage2D(SpriteID, 0, 0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    };

    void Sprite2D::Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    };

    void Sprite2D::Bind( uint32_t slot ) {
        glBindTextureUnit( slot, SpriteID );
    };

    void Sprite2D::Change( const std::string& location ) {
        Path = location;
        int m_TexWidth, m_TexHeight, m_TexChannels;
        unsigned char* m_TextureData = stbi_load(location.c_str( ), &m_TexWidth, &m_TexHeight, NULL, 4);

        printf("location: %s\n", location.c_str());

        if( m_TextureData ) {
            Width = m_TexWidth;
            Height = m_TexHeight;
            
            glTextureStorage2D(SpriteID, 1, GL_RGBA8, m_TexWidth, m_TexHeight );
            
            //glTextureSubImage2D(SpriteID, 0, 0, 0, m_TexWidth, m_TexHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureData);
            SetData(m_TextureData);
            //SetData(m_TextureData);
            //glTextureSubImage2D(SpriteID, 0, 0, 0, m_TexWidth, m_TexHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureData);
            stbi_image_free(m_TextureData);
        };
        printf("should change\n");
    };

    std::string Sprite2D::GetCurrentPath() {
        return Path;
    };
};