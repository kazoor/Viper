#include "orthographic_camera.hpp"

namespace Viper::Renderer {
    OrthoGraphicCamera::OrthoGraphicCamera() {
        m_ProjectionMatrix = glm::mat4(0.0f);
        m_ViewMatrix = glm::mat4(0.0f);
        m_ViewProjectionMatrix = glm::mat4(0.0f);
    };
    OrthoGraphicCamera::OrthoGraphicCamera(float left, float right, float bottom, float top, float z_near, float z_far ) {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, z_near, z_far);
        m_ViewMatrix = glm::mat4(1.0f);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    glm::mat4 OrthoGraphicCamera::GetViewProjectionMatrix() const {
        return m_ViewProjectionMatrix;
    }

    glm::mat4 OrthoGraphicCamera::GetViewMatrix() const {
        return m_ViewMatrix;
    }

    glm::mat4 OrthoGraphicCamera::GetProjectionMatrix() const {
        return m_ProjectionMatrix;
    }

    // TDL
    glm::vec3 OrthoGraphicCamera::ScreenToWorld( glm::vec3 in ) {
        return glm::vec3(0.0f,0.0f,0.0f);
    }

    glm::vec3 OrthoGraphicCamera::WorldToScreen( glm::vec3 in ) {
        return glm::vec3(0.0f,0.0f,0.0f);
    }
};