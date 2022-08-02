#include <glad/glad.h>
#include "api/vertex.hpp"
#include "vertexbuffer.hpp"

namespace Viper::Renderer {
    VertexBuffer::VertexBuffer(uint32_t max_vert_cound) {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, max_vert_cound * sizeof( RendererAPI::Vertex_t ), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t ), ( const void* )offsetof(RendererAPI::Vertex_t, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t ), ( const void* )offsetof(RendererAPI::Vertex_t, color));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t ), ( const void* )offsetof(RendererAPI::Vertex_t, texcoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t ), ( const void* )offsetof(RendererAPI::Vertex_t, texindex));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t ), ( const void* )offsetof(RendererAPI::Vertex_t, tilefactor));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t max_vert_cound) {
        return CreateRef<VertexBuffer>(max_vert_cound);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &VBO);
    }

    void VertexBuffer::Bind() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    void VertexBuffer::Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    uint32_t VertexBuffer::Get() const {
        return VBO;
    };
};