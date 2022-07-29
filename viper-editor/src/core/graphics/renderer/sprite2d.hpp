#pragma once
#include <util/ref/reference.hpp>

namespace Viper::Renderer {
    class Sprite2D {
    public:
        Sprite2D();
        Sprite2D( const std::string& str, int width, int height );
        ~Sprite2D();
        static Ref< Sprite2D > Create( const std::string& sprite_path );
        static Ref< Sprite2D > Create( const std::string& sprite_path, int width, int height );

        uint32_t GetSprite() const;

        void Bind();
        void Unbind();
    private:
        int Width;
        int Height;
        std::string Path;
        uint32_t SpriteID;
    };
};