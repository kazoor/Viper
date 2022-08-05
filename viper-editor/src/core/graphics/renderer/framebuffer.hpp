//
// Created by guwi on 2022-07-26.
//

#ifndef VIPER_FRAMEBUFFER_HPP
#define VIPER_FRAMEBUFFER_HPP
#include <cstdint>
#include <util/ref/reference.hpp>
#include <viper/viper.hpp>

namespace Viper::Renderer {
    class VIPER_API FrameBuffer {
    public:
        FrameBuffer( );
        ~FrameBuffer();

        uint32_t Get() const;
        uint32_t GetColorAttachment() const;
        uint32_t GetDepthAttachment() const;

        void Setup( int width, int height );
        void Bind();
        void Unbind();
        void Resize( int width, int height );

        void BindTexture( uint32_t texture );

        static Ref< FrameBuffer > Create( );
    private:
        uint32_t FBO;
        uint32_t m_ColorAttachment;
        uint32_t m_DepthBufferAttachment;
    };
};

#endif //VIPER_FRAMEBUFFER_HPP
