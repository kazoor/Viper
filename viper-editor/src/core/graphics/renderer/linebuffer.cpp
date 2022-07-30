#include <glad/glad.h>
#include "api/vertex.hpp"
#include "linebuffer.hpp"

namespace Viper::Renderer {
    LineBuffer::LineBuffer(uint32_t max_line_count) {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, max_line_count * sizeof( RendererAPI::LineVertex_t ), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::LineVertex_t ), ( const void* )offsetof(RendererAPI::LineVertex_t, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::LineVertex_t ), ( const void* )offsetof(RendererAPI::LineVertex_t, color));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Ref<LineBuffer> LineBuffer::Create(uint32_t max_line_count) {
        return CreateRef< LineBuffer >( max_line_count );
    }

    LineBuffer::~LineBuffer() {
        glDeleteBuffers(1, &VBO);
    }

    void LineBuffer::Bind() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    void LineBuffer::Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    uint32_t LineBuffer::Get() const {
        return VBO;
    };
};