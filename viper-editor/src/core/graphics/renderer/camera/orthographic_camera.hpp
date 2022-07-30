#pragma once
#include <glm/gtc/matrix_transform.hpp> // ortho

namespace Viper::Renderer {
    class OrthoGraphicCamera {
    public:
        OrthoGraphicCamera();
        OrthoGraphicCamera(float left, float right, float bottom, float top, float z_near = -1.0f, float z_far = 1.0f );

        void SetProjection(float left, float right, float bottom, float top, float z_near = -1.0f, float z_far = 1.0f );
        void SetPosition( const glm::vec3& position );
        glm::mat4 GetViewProjectionMatrix() const;
        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        
        glm::vec3 ScreenToWorld( glm::vec3 in, glm::vec2 screen );
        glm::vec3 WorldToScreen( glm::vec3 in, glm::vec2 screen );
    private:
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;
        glm::mat4 m_ProjectionMatrix; 
    };
};