#pragma once
#include <util/timer/timestep.hpp>
#include <events/event/event.hpp>
#include "orthographic_camera.hpp"

namespace Viper {
    class VIPER_API OrthoGraphicCameraController {
    public:
        OrthoGraphicCameraController(float aspectratio, bool rotation = false);

        void OnUpdate(Timestep::Timestep ts);
        void OnEvent(Events::Event& event);
        const Renderer::OrthoGraphicCamera& GetCamera() const { return m_camera; };
    private:
        bool OnMouseScrolled(Events::MouseScrollEvent& event);
        bool OnWindowResize(Events::WindowResizeEvent& event);
    private:
        float m_aspect_ratio;
        bool m_rotation;
        float m_zoom_level = 1.0f;

        Renderer::OrthoGraphicCamera m_camera;
        glm::vec3 m_camera_position{0.0f,0.0f,0.0f};
        float m_camera_rotation;
        float m_camera_trans_speed{20.0f};
        float m_camera_rot_speed{20.0f};
    };
};