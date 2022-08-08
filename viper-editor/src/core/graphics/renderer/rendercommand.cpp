#include <glad/glad.h>
#include "rendercommand.hpp"
#include <graphics/renderer/framebuffer.hpp>

namespace Viper {
    void RenderCommand::Clear() {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    };

    void RenderCommand::SetColor(const glm::vec4 color) {
        glClearColor(color.x, color.y, color.z, color.w);
    };
};