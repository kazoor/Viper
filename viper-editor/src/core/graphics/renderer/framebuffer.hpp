//
// Created by guwi on 2022-07-26.
//

#ifndef VIPER_FRAMEBUFFER_HPP
#define VIPER_FRAMEBUFFER_HPP
#include <cstdint>
#include <util/ref/reference.hpp>
#include <viper/viper.hpp>

namespace Viper {
    class VIPER_API FrameBuffer {
    public:
        struct FramebufferSpec_t {
            uint32_t width = 0;
            uint32_t height = 0;
            uint32_t samplers = 1;
        };
        FrameBuffer( const FramebufferSpec_t& specifications );
        ~FrameBuffer( );

        uint32_t Get() const;
        uint32_t GetColorAttachment() const;
        uint32_t GetDepthAttachment() const;

        void Setup( int width, int height );
        void Bind();
        void Unbind();
        void Resize( int width, int height );

        void BindTexture( uint32_t texture );

        const FramebufferSpec_t& GetSpecification() const { return specs; };

        static Ref< FrameBuffer > Create( const FramebufferSpec_t& spec );
    private:
        uint32_t FBO;
        uint32_t m_ColorAttachment;
        uint32_t m_DepthBufferAttachment;

        FramebufferSpec_t specs;
    };
};

#endif //VIPER_FRAMEBUFFER_HPP
