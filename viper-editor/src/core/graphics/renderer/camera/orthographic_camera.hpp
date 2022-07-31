#pragma once
#include <viper/viper.hpp>
#include <glm/gtc/matrix_transform.hpp> // ortho

namespace Viper::Renderer {
    class VIPER_API OrthoGraphicCamera {
    public:
        OrthoGraphicCamera();
        OrthoGraphicCamera(float left, float right, float bottom, float top, float z_near = -1.0f, float z_far = 1.0f );

        void SetProjection(float left, float right, float bottom, float top, float z_near = -1.0f, float z_far = 1.0f );
        void SetPosition( const glm::vec3& position );
        glm::mat4 GetViewProjectionMatrix() const;
        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        
        glm::vec2 ScreenToWorld( glm::vec2 in, glm::vec2 screen );
        glm::vec2 Editor_ScreenToWorld( glm::vec2 in, glm::vec2 screen );

        glm::vec2 WorldToScreen( glm::vec2 in, glm::vec2 screen );
    private:
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;
        glm::mat4 m_ProjectionMatrix; 
    };
};