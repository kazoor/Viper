#pragma once
#include "camera.hpp"

// Timestep, needed to smooth / ease things out.
#include <util/timer/timestep.hpp>

// Events, simply to control the mouse & keyboard movement.
#include <events/event/event.hpp>

namespace Viper {
    class VIPER_API EditorCamera : public Camera {
    public:
        EditorCamera();
        EditorCamera(float fov, float near, float far, float ratio);
        virtual ~EditorCamera() = default;

        void OnUpdate(Timestep::Timestep ts); // For smooth movements when panning around.
        void OnViewportResize(uint32_t width, uint32_t height);
        void OnEvent( Events::Event& event );
        glm::mat4 GetTransform();
    private:
        void RecalculateMatrix();

        bool OnMouseScrolled(Events::MouseScrollEvent& event);
        glm::vec3 Lerp( glm::vec3 in, glm::vec3 dir, float ticks );
    private:
        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 2.0f }; 
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraScale = { 1.0f, 1.0f, 1.0f };
		
        float m_CameraFOV = 20.0f;
        float m_CameraNear = -1.0f;
        float m_CameraFar = 1.0f;
        float m_AspectRatio = 0.0f;

        glm::vec3 m_EasedPosition = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_EasedRotation = { 0.0f, 0.0f, 0.0f };
    };
};