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

        static void DrawQuad( const glm::mat4& transform, RendererAPI::Color color );
        static void DrawQuad( const glm::vec2& pos, RendererAPI::Color color );
        static void DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color );

        static void DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color );
        static void DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color );

        void Begin( const OrthoGraphicCamera& camera );
        void Flush();
        void End();

        void BindFramebuffer();
        void UnbindFramebuffer();

        void ResizeFBO( int Width, int Height );

        void PushVec2( const std::string& s, const glm::vec2& v );
        void PushFloat( const std::string& s, float v );

        static uint32_t GetTexture();

        // Stats.
        uint32_t GetVertexCount();
        uint32_t GetIndexCount();
        uint32_t GetQuadCount();

        OrthoGraphicCamera GetCamera() const;
    private:
        uint32_t m_TestTexture;
        OrthoGraphicCamera m_Camera;
    };

    class RenderCommand {
    public:
        static void DrawIndexed( uint32_t Vao, uint32_t IndexCount );
        static void BindFramebuffer( uint32_t Framebuffer );
        static void UnbindFramebuffer( );
        static void ResizeTexture( uint32_t TextureID, int Width, int Height );
        static void Clear();
    };
};