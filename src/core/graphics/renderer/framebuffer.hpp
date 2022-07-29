//
// Created by guwi on 2022-07-26.
//

#ifndef VIPER_FRAMEBUFFER_HPP
#define VIPER_FRAMEBUFFER_HPP
#include <cstdint>
#include <util/ref/reference.hpp>

namespace Viper::Renderer {
    class FrameBuffer {
    public:
        FrameBuffer( );
        ~FrameBuffer();

        uint32_t Get() const;
        uint32_t GetTex() const;

        void Bind();
        void Unbind();

        static Ref< FrameBuffer > Create( );
    private:
        uint32_t FBO;
        uint32_t TEX;
    };
};

#endif //VIPER_FRAMEBUFFER_HPP
