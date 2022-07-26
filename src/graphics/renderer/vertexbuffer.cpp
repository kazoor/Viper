#include "vertexbuffer.hpp"

namespace Viper::Renderer {
    VertexBuffer::VertexBuffer() {

    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t max_vert_cound) {
        return CreateRef<VertexBuffer>();
    }
};