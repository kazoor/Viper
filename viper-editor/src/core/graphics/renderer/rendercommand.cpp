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
        s_Command.Framebuffer->Resize( width, height );
    };

    void RenderCommand::BindFBO() {
        s_Command.Framebuffer->Bind();
    };

    void RenderCommand::UnbindFBO() {
        s_Command.Framebuffer->Unbind();
    };

    uint32_t RenderCommand::GetColorAttachment( ) { return s_Command.Framebuffer->GetColorAttachment();};
    uint32_t RenderCommand::GetDepthAttachment( ) { return s_Command.Framebuffer->GetDepthAttachment();};
};