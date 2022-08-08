#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // ortho
#include <viper/viper.hpp>

namespace Viper {
    class VIPER_API Renderer3D {
    public:
        struct Stats3D_t {
            uint32_t m_IndexCount = 0;
            uint32_t m_CubeCount = 0;
        };
        static void Init();
        static void Shutdown();

        static void Begin(const glm::mat4& camera_transform, const glm::mat4& transform );
        static void End();

        static void Quad();
        static void Quad( const glm::mat4& transform, const glm::vec4& color );

        static Stats3D_t& GetStats();
    private:
        static void Flush();
        static void BeginBatch();
        static void EndBatch();
    };
};