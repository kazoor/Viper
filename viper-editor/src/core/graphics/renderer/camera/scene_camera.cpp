#include "scene_camera.hpp"
#include <glm/gtc/matrix_transform.hpp> // ortho
namespace Viper {
    SceneCamera::SceneCamera() {
        Recalculate();
    };

    void SceneCamera::SetOrthographic( float size, float near, float far ) {
        m_OrthoSize = size;
        m_OrthoNear = near;
        m_OrthoFar = far;

        Recalculate();
    };

    void SceneCamera::SetViewportSize( uint32_t width, uint32_t height ) {
        m_AspectRatio = ( float )width / ( float )height;
        Recalculate();
    };

    void SceneCamera::Recalculate() {
        float ortho_left = -m_OrthoSize * m_AspectRatio * 0.5f;
        float ortho_right = m_OrthoSize * m_AspectRatio * 0.5f;
        float ortho_bottom = -m_OrthoSize * 0.5f;
        float ortho_top = m_OrthoSize * 0.5f;

        m_Projection = glm::ortho( ortho_left, ortho_right, ortho_bottom, ortho_top, m_OrthoNear, m_OrthoFar );
    };
};