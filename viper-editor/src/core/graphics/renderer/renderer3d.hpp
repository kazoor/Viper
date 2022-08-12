#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // ortho
#include <viper/viper.hpp>
#include <graphics/shaders/shader/shader.hpp>
#include <vector>

namespace Viper {
    class Mesh {
    public:
        std::vector< glm::vec4 > transform;
        std::vector< glm::vec3 > normals;
        std::vector< uint32_t > indices;
        std::vector< glm::vec3 > colors;

        bool mesh_is_constructed = false;
    };

    Mesh* LoadMeshFromPath( const char* path );
    void FreeMesh(Mesh* m);
    class VIPER_API Renderer3D {
    public:
        struct Stats3D_t {
            uint32_t m_IndexCount = 0;
            uint32_t m_CubeCount = 0;
            uint32_t m_DrawCalls = 0;

            uint32_t m_LightSources = 0;
        };
        static void Init();
        static void Shutdown();

        static void Begin(const glm::mat4& camera_transform, const glm::mat4& transform );
        static void Quad( const glm::mat4& transform, const glm::vec4& color );
        static void End();

        static void SetLightPosition( const glm::mat4& transform, const glm::vec4& color, float intensity);
        static void SetLightPosition( const glm::mat4& transform, const glm::vec3& pos, const glm::vec4& color, float intensity);

        static void DrawMesh( const glm::mat4& transform, Mesh* mesh );
        static void InvalidateMesh(Mesh* mesh);

        static Stats3D_t& GetStats();
    private:
        static void Flush();
        static void BeginBatch();
    };
};