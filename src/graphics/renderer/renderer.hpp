#pragma once
#include "api/vertex.hpp" // vec3 redan inkluderad inuti vertex.hpp
#include <glm/vec2.hpp>
#include "camera/orthographic_camera.hpp"
#include <string>

namespace Viper::Renderer {
    
    class Renderer2D {
    public:
        static void Instantiate();
        static void Destroy();

        static void DrawQuad( const glm::mat4& transform, RendererAPI::Color color );
        static void DrawQuad( const glm::vec2& pos, RendererAPI::Color color );
        static void DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color );

        static void DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color );
        static void DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color );

        static void Begin( const OrthoGraphicCamera& camera );
        static void Flush();
        static void End();

        static void BindFramebuffer();
        static void UnbindFramebuffer();

        static void ResizeFBO( int Width, int Height );

        static uint32_t GetTexture();

        // Stats.
        uint32_t GetVertexCount();
        uint32_t GetIndexCount();
        uint32_t GetQuadCount();

    private:
        OrthoGraphicCamera m_Camera;
    };

    class RenderCommand {
    public:
        static void DrawIndexed( uint32_t Vao, uint32_t IndexCount );
        static void BindFramebuffer( uint32_t Framebuffer );
        static void UnbindFramebuffer( );
        static void ResizeTexture( uint32_t TextureID, int Width, int Height );
        static void Clear();
        static void SetClearColor(float color[3]);
    };
};