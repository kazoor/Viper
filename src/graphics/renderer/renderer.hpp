#pragma once
#include "api/vertex.hpp" // vec3 redan inkluderad inuti vertex.hpp
#include <glm/vec2.hpp>
#include "camera/orthographic_camera.hpp"
#include <string>
namespace Viper::Renderer {
    class Renderer2D {
    public:
        Renderer2D();
        ~Renderer2D();
        void DrawQuad( const glm::vec2& pos, RendererAPI::Color color );
        void DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color );

        void DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color );
        void DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color );

        void Begin( const OrthoGraphicCamera& camera );
        void Flush();
        void End();

        void BindFramebuffer();
        void UnbindFramebuffer();

        uint32_t GetFramebufferID() { return m_Fbo; };
        uint32_t GetTexturebufferID() { return m_Tcb; };

        void ResizeFBO( int Width, int Height );

        void PushVec2( const std::string& s, const glm::vec2& v );
        void PushFloat( const std::string& s, float v );

        // Stats.
        uint32_t GetVertexCount();
        uint32_t GetIndexCount();
        uint32_t GetQuadCount();

        OrthoGraphicCamera GetCamera() const;
    private:
        uint32_t m_Vao;
        uint32_t m_Vbo;
        uint32_t m_Ibo;
        uint32_t m_Fbo; // Viewport impl snart.
        uint32_t m_Tcb;
        OrthoGraphicCamera m_Camera;
    };

    class RenderCommand {
    public:
        static void DrawIndexed( uint32_t Vao, uint32_t IndexCount );
    };
};