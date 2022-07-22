#include "renderer.hpp"
#include <glm/mat4x4.hpp>

constexpr uint32_t max_quad_count = 1000;
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
    Renderer::Renderer()
    {
        m_Vao = 0;
        m_Vbo = 0;
        m_Ibo = 0;
        m_Fbo = 0;
        s_Renderer.m_VertexBuffer = new Viper::RendererAPI::Vertex_t[ max_quad_count ];
    }

    Renderer::~Renderer()
    {
        delete[] s_Renderer.m_VertexBuffer;
    }

    void Renderer::DrawQuad( const glm::vec2& pos, RendererAPI::Color color ) {
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
    
    void Renderer::DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color ) {
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

    void Renderer::DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color ) {};
    void Renderer::DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color ) {};

    void Renderer::Begin() {};
    void Renderer::End() {};
};