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
    };

    void OrthoGraphicCamera::SetProjection(float left, float right, float bottom, float top, float z_near, float z_far ) {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, z_near, z_far);
        m_ViewMatrix = glm::mat4(1.0f);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    };

    void OrthoGraphicCamera::Recalculate() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_CameraPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        m_ViewMatrix = glm::inverse( transform );
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    };

    void OrthoGraphicCamera::SetPosition( const glm::vec3& position ) {
        m_CameraPosition = position;
        Recalculate();
    };

    void OrthoGraphicCamera::SetRotation( const float& rotation ) {
        m_Rotation = rotation;
        Recalculate();
    };

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
    glm::vec2 OrthoGraphicCamera::ScreenToWorld( glm::vec2 in, glm::vec2 screen ) {
        return glm::inverse(m_ProjectionMatrix * m_ViewMatrix) 
        * ((glm::vec4(in.x ,in.y, 1.0f, 1.0f) - glm::vec4(screen.x / 2.0f, screen.y / 2.0f, 0.0f, 0.0f)) 
        / glm::vec4(screen.x / 2.0f, -screen.y / 2.0f, 1.0f, 1.0f));
    }

    glm::vec2 OrthoGraphicCamera::Editor_ScreenToWorld( glm::vec2 in, glm::vec2 screen ) {
        return glm::inverse(m_ProjectionMatrix * m_ViewMatrix) 
        * ((glm::vec4(in.x ,in.y, 1.0f, 1.0f) - glm::vec4(screen.x / 2.0f, screen.y / 2.0f, 0.0f, 0.0f)) 
        / glm::vec4(screen.x / 2.0f, screen.y / 2.0f, 1.0f, 1.0f));
    }

    glm::vec2 OrthoGraphicCamera::WorldToScreen( glm::vec2 in, glm::vec2 screen ) {
        return ((m_ProjectionMatrix * m_ViewMatrix) * glm::vec4(in.x, in.y, 1.0f, 1.0f)) 
        * glm::vec4(screen.x / 2.0f, -screen.y / 2.0f, 1.0f, 1.0f) 
        + glm::vec4(screen.x / 2.0f, screen.y / 2.0f, 0.0f, 0.0f);
    }
};