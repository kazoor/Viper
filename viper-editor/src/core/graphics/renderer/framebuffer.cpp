//
// Created by guwi on 2022-07-26.
//
#include <glad/glad.h>
#include "framebuffer.hpp"

namespace Viper::Renderer {
    FrameBuffer::FrameBuffer( ) {
        //glGenFramebuffers(1, &FBO);
        Setup(1280, 720);
    };

    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthBufferAttachment);
    };

    uint32_t FrameBuffer::GetColorAttachment() const {
        return m_ColorAttachment;
    };
    uint32_t FrameBuffer::GetDepthAttachment() const {
        return m_DepthBufferAttachment;
    };

    uint32_t FrameBuffer::Get() const {
        return FBO;
    };

    void FrameBuffer::Setup( int width, int height ) {
        if( FBO ) {
            glDeleteFramebuffers(1, &FBO);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthBufferAttachment);
        };
        glCreateFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glBindTexture(GL_TEXTURE_2D, 0);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBufferAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthBufferAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height );

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferAttachment, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    };

    void FrameBuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    };

    void FrameBuffer::Resize( int width, int height ) {
        Setup( width, height );
    };

    Ref<FrameBuffer> FrameBuffer::Create() {
        return CreateRef<FrameBuffer>();
    };
};