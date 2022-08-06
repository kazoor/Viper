#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // ortho
#include <graphics/renderer/camera/orthographic_camera.hpp>
#include <viper/viper.hpp>

namespace Viper {
    class VIPER_API Light2D {
    public:
        Light2D();
        ~Light2D();
        void Begin( const Renderer::OrthoGraphicCamera& camera );
        void Light( const glm::vec2& position, glm::vec4 color, float angle = 0.0f, const glm::vec2& size = glm::vec2(1.0f));
        void End();
    private:
        uint32_t m_LightVAO;
        uint32_t m_LightVBO;
        Renderer::OrthoGraphicCamera m_camera;
    };
};