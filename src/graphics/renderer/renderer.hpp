#pragma once
#include "api/vertex.hpp" // vec3 redan inkluderad inuti vertex.hpp
#include <glm/vec2.hpp>

namespace Viper::Renderer {
    class Renderer2D {
    public:
        Renderer2D();
        ~Renderer2D();
        void DrawQuad( const glm::vec2& pos, RendererAPI::Color color );
        void DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color );

        void DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color );
        void DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color );

        void FboBegin();
        void FboEnd();
        void Begin();
        void Flush();
        void End();

        uint32_t GetFramebufferID() { return m_Fbo; };
        uint32_t GetTexturebufferID() { return m_Tcb; };

        // Stats.
        uint32_t GetVertexCount();
        uint32_t GetIndexCount();
        uint32_t GetQuadCount();
    private:
        uint32_t m_Vao;
        uint32_t m_Vbo;
        uint32_t m_Ibo;
        uint32_t m_Fbo; // Viewport impl snart.
        uint32_t m_Tcb;
    };
};