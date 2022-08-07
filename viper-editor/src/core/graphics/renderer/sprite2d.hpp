#pragma once
#include <string.h>
#include <cinttypes>
#include <util/ref/reference.hpp>
#include <viper/viper.hpp>

namespace Viper {
    class VIPER_API Sprite2D {
    public:
        Sprite2D();
        Sprite2D( const std::string& str );
        Sprite2D( int width, int height );
        ~Sprite2D();

        static Ref< Sprite2D > Create( const std::string& sprite_path );
        static Ref< Sprite2D > Create( int width, int height );

        uint32_t GetSprite() const;
        
        void Bind();
        void Unbind();
        void Bind( uint32_t slot );
        void SetData( void* data );
        void Delete();
        void Change( const std::string& location );

        std::string GetCurrentPath();

        int GetWidth() const;
        int GetHeight() const;
        std::string GetPath() const;
        uint32_t GetSpriteID() const;

        const int SetWidth(const int W);
        const int SetHeight(const int H);
        const std::string SetPath(const std::string &Path);
        const uint32_t SetSpriteID(const std::uint32_t &ID);

    private:
        int Width;
        int Height;
        std::string Path;
        uint32_t SpriteID;
    };
};