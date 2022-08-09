#include "orthographic_camera_controller.hpp"
#include <util/input/input.hpp>
#include <util/input/keycodes.hpp>
#include <util/globals/global.hpp>

namespace Viper {
    OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectratio, bool rotation) :
    m_aspect_ratio(aspectratio), m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level), m_rotation(rotation), m_camera_rotation( 0.0f ) {

    };

    void OrthoGraphicCameraController::OnUpdate(Timestep::Timestep ts) {
        if(!Globals::Editor::IsSceneHovered)
            return;

        if(Input::Input::IsKeyPressed(VIPER_KEY_A))
            m_camera_position.x -= m_camera_trans_speed * ts.deltatime();

        if(Input::Input::IsKeyPressed(VIPER_KEY_D))
            m_camera_position.x += m_camera_trans_speed * ts.deltatime();

        if(Input::Input::IsKeyPressed(VIPER_KEY_W))
            m_camera_position.y += m_camera_trans_speed * ts.deltatime();

        if(Input::Input::IsKeyPressed(VIPER_KEY_S))
            m_camera_position.y -= m_camera_trans_speed * ts.deltatime();

        if( m_rotation ) {
            if( Input::Input::IsKeyPressed(VIPER_KEY_Q))
                m_camera_rotation += m_camera_rot_speed * ts.deltatime();

            if( Input::Input::IsKeyPressed(VIPER_KEY_E))
                m_camera_rotation -= m_camera_rot_speed * ts.deltatime();

            m_camera.SetRotation(m_camera_rotation);
        };

        m_camera.SetPosition(m_camera_position);
    };

    void OrthoGraphicCameraController::OnEvent(Events::Event& event) {
        Events::EventDispatcher dispatcher(event);

        dispatcher.Dispatch< Events::MouseScrollEvent >( VIPER_GET_EVENT_FUNC(OrthoGraphicCameraController::OnMouseScrolled ));
        dispatcher.Dispatch< Events::WindowResizeEvent >( VIPER_GET_EVENT_FUNC(OrthoGraphicCameraController::OnWindowResize ));
    };

    bool OrthoGraphicCameraController::OnMouseScrolled(Events::MouseScrollEvent& event) {
        printf("scroll event.\n");
        m_zoom_level = std::max( m_zoom_level, 1.0f);
        m_zoom_level -= event.y;
        m_camera.SetProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
        return true;
    };

    bool OrthoGraphicCameraController::OnWindowResize(Events::WindowResizeEvent& event) {
        printf("scroll event.\n");
        m_aspect_ratio = ( float )event.Width / ( float )event.Height;
        m_camera.SetProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
        return true;
    };

    void OrthoGraphicCameraController::SetPosition( const glm::vec3& position ) {
        m_camera_position = position;
        m_camera.SetPosition( m_camera_position );
    };
};