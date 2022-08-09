#pragma once
#include <util/ref/reference.hpp>
#include <viper/viper.hpp>
#include <graphics/renderer/framebuffer.hpp>

namespace Viper {
    class VIPER_API ApplicationBuffer {
    public:
        ApplicationBuffer();
        ~ApplicationBuffer();

        void Render( const Ref< FrameBuffer >& framebuffer );

        static Ref< ApplicationBuffer > Create();
    private:
        uint32_t m_Vao;
        uint32_t m_Vbo;
        uint32_t m_Ebo;
    };
};