#include "renderer.hpp"
#include <glm/mat4x4.hpp>
#include <glad/glad.h>

constexpr uint32_t max_quad_count = 32000;
constexpr uint32_t max_vertices_count = max_quad_count * 4;
constexpr uint32_t max_indices_count = max_quad_count * 6;

struct RendererData {
    Viper::RendererAPI::Vertex_t* m_VertexBuffer = nullptr;
    Viper::RendererAPI::Vertex_t* m_VertexBufferPtr = nullptr;

    uint32_t m_IndexCount = 0;
    uint32_t m_QuadCount = 0;
    uint32_t m_VertexCount = 0;
};
RendererData s_Renderer;

namespace Viper::Renderer {
    Renderer2D::Renderer2D()
    {
        m_Vao = 0;
        m_Vbo = 0;
        m_Ibo = 0;
        m_Fbo = 0;

        s_Renderer.m_VertexBuffer = new Viper::RendererAPI::Vertex_t[ max_quad_count ];
        
        glGenVertexArrays(1, &m_Vao);
        glBindVertexArray(m_Vao);

        glGenBuffers(1, &m_Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, max_vertices_count * sizeof(RendererAPI::Vertex_t), nullptr, GL_DYNAMIC_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t), ( const void* )offsetof(RendererAPI::Vertex_t, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t), ( const void* )offsetof(RendererAPI::Vertex_t, color));
    
        uint32_t m_IndexArray[ max_indices_count ];
        uint32_t m_IndexOffset = 0;

        for( size_t index = 0; index < max_indices_count; index += 6 ) {
            m_IndexArray[ index + 0 ] = 0 + m_IndexOffset;
            m_IndexArray[ index + 1 ] = 1 + m_IndexOffset;
            m_IndexArray[ index + 2 ] = 2 + m_IndexOffset;
            m_IndexArray[ index + 3 ] = 2 + m_IndexOffset;
            m_IndexArray[ index + 4 ] = 3 + m_IndexOffset;
            m_IndexArray[ index + 5 ] = 0 + m_IndexOffset;

            m_IndexOffset += 4;
        };

        glGenBuffers(1, &m_Ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IndexArray), m_IndexArray, GL_STATIC_DRAW);

        glBindVertexArray(0);

        glGenFramebuffers(1, &m_Fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

        // TESTING. (Skapa textur.)
        glGenTextures(1, &m_Tcb);
        glBindTexture(GL_TEXTURE_2D, m_Tcb);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Tcb, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Renderer2D::~Renderer2D()
    {
        delete[] s_Renderer.m_VertexBuffer;
        glDeleteFramebuffers(1, &m_Fbo);
        glDeleteVertexArrays(1, &m_Vao);
        glDeleteTextures(1, &m_Tcb);
        glDeleteBuffers(1, &m_Vbo);
        glDeleteBuffers(1, &m_Ibo);
    }

    void Renderer2D::Flush()
    {
        GLsizeiptr size_ptr = reinterpret_cast< uint8_t* >( s_Renderer.m_VertexBufferPtr ) - reinterpret_cast< uint8_t* >( s_Renderer.m_VertexBuffer );
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, s_Renderer.m_VertexBuffer );
    }

    void Renderer2D::DrawQuad( const glm::vec2& pos, RendererAPI::Color color ) {
        s_Renderer.m_VertexBufferPtr->position = glm::vec3(pos.x, pos.y, 0.0f);
        s_Renderer.m_VertexBufferPtr->color = color;
        s_Renderer.m_VertexBufferPtr++;

        s_Renderer.m_VertexBufferPtr->position = glm::vec3(pos.x + 1.0f, pos.y, 0.0f);
        s_Renderer.m_VertexBufferPtr->color = color;
        s_Renderer.m_VertexBufferPtr++;
        
        s_Renderer.m_VertexBufferPtr->position = glm::vec3(pos.x + 1.0f, pos.y + 1.0f, 0.0f);
        s_Renderer.m_VertexBufferPtr->color = color;
        s_Renderer.m_VertexBufferPtr++;

        s_Renderer.m_VertexBufferPtr->position = glm::vec3(pos.x, pos.y + 1.0f, 0.0f);
        s_Renderer.m_VertexBufferPtr->color = color;
        s_Renderer.m_VertexBufferPtr++;

        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };
    
    void Renderer2D::DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color ) {
        s_Renderer.m_VertexBufferPtr->position = glm::vec3(pos.x, pos.y, 0.0f);
        s_Renderer.m_VertexBufferPtr->color = color;
        s_Renderer.m_VertexBufferPtr++;

        s_Renderer.m_VertexBufferPtr->position = glm::vec3(pos.x + size.x, pos.y, 0.0f);
        s_Renderer.m_VertexBufferPtr->color = color;
        s_Renderer.m_VertexBufferPtr++;
        
        s_Renderer.m_VertexBufferPtr->position = glm::vec3(pos.x + size.x, pos.y + size.y, 0.0f);
        s_Renderer.m_VertexBufferPtr->color = color;
        s_Renderer.m_VertexBufferPtr++;

        s_Renderer.m_VertexBufferPtr->position = glm::vec3(pos.x, pos.y + size.y, 0.0f);
        s_Renderer.m_VertexBufferPtr->color = color;
        s_Renderer.m_VertexBufferPtr++;

        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };

    void Renderer2D::DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color ) {};
    void Renderer2D::DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color ) {};

    void Renderer2D::Begin() {
        s_Renderer.m_VertexBufferPtr = s_Renderer.m_VertexBuffer;
        //glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
    };

    void Renderer2D::End() {
        
        glBindVertexArray(m_Vao);
        //glBindTexture(GL_TEXTURE_2D, m_Tcb);
        glDrawElements(GL_TRIANGLES, s_Renderer.m_IndexCount, GL_UNSIGNED_INT, nullptr );
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        s_Renderer.m_IndexCount = 0;
        s_Renderer.m_QuadCount = 0;
        s_Renderer.m_VertexCount = 0;
    };

    void Renderer2D::FboBegin() {
    }

    void Renderer2D::FboEnd() {
    }

    uint32_t Renderer2D::GetVertexCount() { return s_Renderer.m_VertexCount; }
    uint32_t Renderer2D::GetIndexCount() { return s_Renderer.m_IndexCount; }
    uint32_t Renderer2D::GetQuadCount() { return s_Renderer.m_QuadCount; }
};