#pragma once
#include "../../util/ref/reference.hpp"

namespace Viper::Renderer {
    class Sprite2D {
    public:
        Sprite2D( const std::string& str, int width, int height );
        static Ref< Sprite2D > Create( const std::string& sprite_path );
        static Ref< Sprite2D > Create( const std::string& sprite_path, int width, int height );
    private:
        int Width;
        int Height;
        std::string Path;
    };
};