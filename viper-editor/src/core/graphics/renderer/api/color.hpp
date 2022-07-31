#pragma once
#include <cstring>
#include <viper/viper.hpp>

namespace Viper::RendererAPI {
    struct VIPER_API Color {
        float colors[ 4U ];
        Color() {
            memset( colors, 0, sizeof( colors ) );
        };

        Color( int red, int green, int blue, int alpha = 255 ) {
            SetColor(red, green, blue, alpha);
        };

        Color( float red, float green, float blue, float alpha = 1.0f ) {
           SetColor(red, green, blue, alpha);
        };

        inline void SetColor(int red, int green, int blue, int alpha = 255) {
            colors[ 0 ] = red / 255.0f;
            colors[ 1 ] = green / 255.0f;
            colors[ 2 ] = blue / 255.0f;
            colors[ 3 ] = alpha / 255.0f;
        };

        inline void SetColor(float red, float green, float blue, float alpha = 1.0f) {
            colors[ 0 ] = red;
            colors[ 1 ] = green;
            colors[ 2 ] = blue;
            colors[ 3 ] = alpha;
        };

        inline static Color White( void ) {
            return Color( 1.0f, 1.0f, 1.0f, 1.0f );
        };

        inline static Color Black( void ) {
            return Color( 0.0f, 0.0f, 0.0f, 1.0f );
        };

        inline static Color Red( void ) {
            return Color( 1.0f, 0.0f, 0.0f, 1.0f );
        };

        inline static Color Green( void ) {
            return Color( 0.0f, 1.0f, 0.0f, 1.0f );
        };

        inline static Color Blue( void ) {
            return Color( 0.0f, 0.0f, 1.0f, 1.0f );
        };

        inline bool operator==( const Color& color ) {
            return ( colors == color.colors );
        };

        inline bool operator!=( const Color& color ) {
            return ( colors != color.colors );
        };
    };
};