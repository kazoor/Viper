#include "sprite2d.hpp"

namespace Viper::Renderer {
    Sprite2D::Sprite2D( const std::string& path, int width, int height ) : Path(path), Width( width ), Height( height ) { };

    Ref< Sprite2D > Sprite2D::Create( const std::string& sprite_path ) {
        return CreateRef< Sprite2D >( sprite_path, 1, 1 );
    };

    Ref< Sprite2D > Sprite2D::Create( const std::string& sprite_path, int width, int height ) {
        return CreateRef< Sprite2D >( sprite_path, width, height );
    };
};