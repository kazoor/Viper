#include <glad/glad.h>
#include "vertex.hpp"

namespace Viper {

    // <!-- Begin Index Buffer
    IndexBuffer::IndexBuffer(uint32_t* data, size_t size) : m_Count( size ) {
        glCreateBuffers(1, &m_Ebo);

        glBindBuffer(GL_ARRAY_BUFFER, m_Ebo);

        glBufferData(GL_ARRAY_BUFFER, size * sizeof( uint32_t ), data, GL_STATIC_DRAW);
    };

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_Ebo);
    };

    void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo); };

    void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

    uint32_t IndexBuffer::GetCount() const { return m_Count; };

    Ref< IndexBuffer > IndexBuffer::Create( uint32_t* data, size_t size ) {
        return CreateRef< IndexBuffer >( data, size );
    };

    // End Index Buffer --!>

    // <!-- Begin Vertex Buffer
    VertexBuffer::VertexBuffer(const uint32_t size) {
        glCreateBuffers(1, &m_Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo );

        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    };

     VertexBuffer::VertexBuffer( float* vertices, const uint32_t maxbuffer ) {
        glCreateBuffers(1, &m_Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo );
        glBufferData(GL_ARRAY_BUFFER, maxbuffer, vertices, GL_STATIC_DRAW );
    };

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_Vbo);
    };

    void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_Vbo); };

    void VertexBuffer::Unind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); };

    void VertexBuffer::SetData(const void* ptr, uint32_t size ) {
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, ptr );
    };

    Ref< VertexBuffer > VertexBuffer::Create(const uint32_t size) {
        return CreateRef< VertexBuffer >( size );
    };

    Ref< VertexBuffer > VertexBuffer::Create(float* vertices, const uint32_t maxbuffer) {
        return CreateRef< VertexBuffer >( vertices, maxbuffer );
    };
    // End Vertex Buffer --!>

    VertexArray::VertexArray() {
        glCreateVertexArrays(1, &m_Vao);
    };

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_Vao);
    };

    void VertexArray::SetIndexInfo( const Ref< IndexBuffer >& indexbuffer ) {
        glBindVertexArray(m_Vao);
        indexbuffer->Bind();

        m_indexbuffer = indexbuffer;
    };

    Ref< IndexBuffer > VertexArray::GetIndexBuffer() const { return m_indexbuffer; };

    void VertexArray::Bind() const { glBindVertexArray(m_Vao); };

    void VertexArray::Unbind() const { glBindVertexArray(0); };

    Ref< VertexArray > VertexArray::Create( ) {
        return CreateRef< VertexArray >( );
    };

    UniformBuffer::UniformBuffer( uint32_t size, uint32_t binding ) {
        glCreateBuffers(1, &m_Ubo);
        glNamedBufferData(m_Ubo, size, nullptr, GL_DYNAMIC_DRAW);
	    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_Ubo);
	};

    UniformBuffer::~UniformBuffer() {
        glDeleteBuffers(1, &m_Ubo);
    };

    void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
        glNamedBufferSubData(m_Ubo, offset, size, data);
    };

    Ref< UniformBuffer > UniformBuffer::Create(uint32_t size, uint32_t binding) {
        return CreateRef< UniformBuffer >( size, binding );
    };
}