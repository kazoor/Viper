#include <glad/glad.h>
#include "rendercommand.hpp"
#include <graphics/renderer/framebuffer.hpp>

namespace Viper {
    struct RenderCommand_t {
        Ref< Renderer::FrameBuffer > Framebuffer;
    };
    RenderCommand_t s_Command;

    void RenderCommand::Init() {
        s_Command.Framebuffer = Renderer::FrameBuffer::Create( );
    };

    void RenderCommand::Clear() {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    };

    void RenderCommand::SetColor(const glm::vec4 color) {
        glClearColor(color.x, color.y, color.z, color.w);
    };

    void RenderCommand::Resize(const int width, const int height) {
        glBindTexture(GL_TEXTURE_2D, s_Command.Framebuffer->GetTex());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    };

    void RenderCommand::BindFBO() {
        s_Command.Framebuffer->Bind();
    };

    void RenderCommand::UnbindFBO() {
        s_Command.Framebuffer->Unbind();
    };

    uint32_t RenderCommand::FrameID() {
        return s_Command.Framebuffer->GetTex();
    };
};