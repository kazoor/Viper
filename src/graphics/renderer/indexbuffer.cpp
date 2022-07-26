//
// Created by guwi on 2022-07-26.
//
#include <glad/glad.h>
#include "indexbuffer.hpp"

namespace Viper::Renderer {
    IndexBuffer::IndexBuffer( uint32_t max_index_count ) {
        uint32_t m_IndexArray[ max_index_count ];
        uint32_t m_IndexOffset = 0;

        for( size_t index = 0; index < max_index_count; index += 6 ) {
            m_IndexArray[ index + 0 ] = 0 + m_IndexOffset;
            m_IndexArray[ index + 1 ] = 1 + m_IndexOffset;
            m_IndexArray[ index + 2 ] = 2 + m_IndexOffset;
            m_IndexArray[ index + 3 ] = 2 + m_IndexOffset;
            m_IndexArray[ index + 4 ] = 3 + m_IndexOffset;
            m_IndexArray[ index + 5 ] = 0 + m_IndexOffset;

            m_IndexOffset += 4;
        };

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IndexArray), m_IndexArray, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    };

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &IBO);
    };

    uint32_t IndexBuffer::Get() const {
        return IBO;
    };

    void IndexBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    };

    void IndexBuffer::Unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    };

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t max_index_count) {
        return CreateRef<IndexBuffer>(max_index_count);
    };
};