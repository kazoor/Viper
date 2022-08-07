#define STB_IMAGE_IMPLEMENTATION
#include <cinttypes>
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
        stbi_set_flip_vertically_on_load(1);
        int m_texture_width, m_texture_height, m_texture_channels;
        unsigned char* m_TextureData = stbi_load(path.c_str( ), &m_texture_width, &m_texture_height, &m_texture_channels, 0);
        if( m_TextureData )
            Globals::ConsoleContext::AddLog( VIPER_ICON_SUCC " Loaded texture.", VIPER_FORMAT_STRING( "Loaded texture: %s, dimensions: %dx%d", path.c_str( ), m_texture_width, m_texture_height ), Globals::ConsoleFlag::ConsoleSuccess );
        else
            Globals::ConsoleContext::AddLog( VIPER_ICON_ERR " Unable to load texture.", VIPER_FORMAT_STRING( "Unable to locate: %s, no such file or directory.", path.c_str( ) ), Globals::ConsoleFlag::ConsoleError );
        
        if( m_TextureData ) {
            Width = m_texture_width;
            Height = m_texture_height;

            glCreateTextures(GL_TEXTURE_2D, 1, &SpriteID);
            glBindTexture(GL_TEXTURE_2D, SpriteID);

            GLenum m_internal_format = 0, m_external_format = 0;
            if( m_texture_channels == 4 ) { // r, g, b, a
                m_internal_format = GL_RGBA8;
                m_external_format = GL_RGBA;
            } else if( m_texture_channels == 3 ) { // r, g, b
                m_internal_format = GL_RGB8;
                m_external_format = GL_RGB;
            };

            glTextureStorage2D(SpriteID, 1, m_internal_format, Width, Height );

            glTextureParameteri(SpriteID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(SpriteID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(SpriteID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(SpriteID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D(SpriteID, 0, 0, 0, Width, Height, m_external_format, GL_UNSIGNED_BYTE, m_TextureData);

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

        int m_texture_width, m_texture_height, m_texture_channels;
        unsigned char* m_texture_data = stbi_load( location.c_str( ), &m_texture_width, &m_texture_height, &m_texture_channels, 0);

        if( m_texture_data ) {
            GLenum m_external_format = ( m_texture_channels == 4 ) ? GL_RGBA : GL_RGB;
            glTextureSubImage2D(SpriteID, 0, 0, 0, Width, Height, m_external_format, GL_UNSIGNED_BYTE, m_texture_data);
            stbi_image_free(m_texture_data);
        };
    };

    std::string Sprite2D::GetCurrentPath() {
        return Path;
    };

    int Sprite2D::GetWidth() const {
        return Width;
    }

    int Sprite2D::GetHeight() const {
        return Height;
    }

    std::string Sprite2D::GetPath() const {
        return Path;
    }

    uint32_t Sprite2D::GetSpriteID() const {
        return SpriteID;
    }

    const int Sprite2D::SetWidth(const int W) {
        Width = W;
    }

    const int Sprite2D::SetHeight(const int H) {
        Height = H;
    }

    const std::string Sprite2D::SetPath(const std::string &Pth) {
        Path = Pth;
    }

    const uint32_t Sprite2D::SetSpriteID(const std::uint32_t &ID) {
        SpriteID = ID;
    }
};