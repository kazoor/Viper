#pragma once
#include "api/vertex.hpp" // vec3 redan inkluderad inuti vertex.hpp
#include <glm/vec2.hpp>

namespace Viper::Renderer {
    class Renderer {
    public:
        Renderer();
        ~Renderer();
        void DrawQuad( const glm::vec2& pos, RendererAPI::Color color );
        void DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color );

        void DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color );
        void DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color );

        void Begin();
        void End();
    private:
        uint32_t m_Vao;
        uint32_t m_Vbo;
        uint32_t m_Ibo;
        uint32_t m_Fbo; // Viewport impl snart.
    };
};