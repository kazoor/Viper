#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // ortho
#include <graphics/renderer/camera/orthographic_camera.hpp>
#include <graphics/renderer/sprite2d.hpp>
#include <viper/viper.hpp>

namespace Viper {
    struct Stats {
        uint32_t calls_this_frame;
        uint32_t max_quads_allowed;
        uint32_t max_indices_allowed;
        uint32_t max_vertices_allowed;
    };
    class VIPER_API Renderer2D {
    public:
        static void Init();
        static void Shutdown();

        static void DrawQuad( const glm::mat4& transform, glm::vec4 color = glm::vec4(1.0f) );
        static void DrawQuad( const glm::vec2& pos, const glm::vec2& size, glm::vec4 color = glm::vec4(1.0f) );
        static void DrawOutlinedQuad( const glm::vec2& pos, const glm::vec2& size, glm::vec4 color = glm::vec4(1.0f) );

        static void DrawQuadRotated(const glm::vec2& pos, const glm::vec2& size, float angle = 0.0f, glm::vec4 color = glm::vec4(1.0f));
        static void DrawLine( const glm::vec2& pos0, const glm::vec2& pos1, glm::vec4 color = glm::vec4( 1.0f ) );

        static void DrawTexture( const glm::mat4& transform, const Ref< Sprite2D >& sprite, float tiling, glm::vec4 color = glm::vec4(1.0f));
        static void DrawTexture( const glm::vec2& pos, const glm::vec2& size, const Ref< Sprite2D >& sprite, float tiling = 1.0f, glm::vec4 color = glm::vec4(1.0f));
        static void DrawRotatedTexture( const glm::vec2& pos, const glm::vec2& size, const Ref< Sprite2D >& sprite, float tiling = 1.0f, float angle = 0.0f, glm::vec4 color = glm::vec4(1.0f));

        static void Begin( const Renderer::OrthoGraphicCamera& camera );
        static void End();

        static Stats& GetStats();
    private:
        static void Flush();
        static void BeginBatch();
        static void EndBatch();
    };
};