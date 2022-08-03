//
// Created by guwi on 2022-07-26.
//
#include <glad/glad.h>
#include "framebuffer.hpp"

namespace Viper::Renderer {
    FrameBuffer::FrameBuffer( ) {
        //glGenFramebuffers(1, &FBO);
        glCreateFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glGenTextures(1, &TEX);
        glBindTexture(GL_TEXTURE_2D, TEX);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEX, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    };

    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &TEX);
    };

    uint32_t FrameBuffer::Get() const {
        return FBO;
    };

    uint32_t FrameBuffer::GetTex() const {
        return TEX;
    };

    void FrameBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    };

    void FrameBuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    };

    Ref<FrameBuffer> FrameBuffer::Create() {
        return CreateRef<FrameBuffer>();
    };
};