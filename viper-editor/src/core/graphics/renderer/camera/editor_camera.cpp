#include "editor_camera.hpp"

// Get the required functions, such as glm::perspective and glm::ortho
// perspective for perspective view.
// ortho for orthographic view.
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <util/input/input.hpp>

#include <util/input/keycodes.hpp>

namespace Viper {
    using namespace Input;
    EditorCamera::EditorCamera() { RecalculateMatrix(); };
    EditorCamera::EditorCamera(float fov, float near, float far, float ratio) {
        m_CameraFar = far;
        m_CameraNear = near;
        m_CameraFOV = fov;
        m_AspectRatio = ratio;

        RecalculateMatrix();
    };

    void EditorCamera::OnViewportResize(uint32_t width, uint32_t height) {
        m_AspectRatio = (float)width / (float)height;
        RecalculateMatrix();
    };

    void EditorCamera::OnUpdate(Timestep::Timestep ts) {
        float m_DirSpeedX = 0.0f;
        float m_DirSpeedY = 0.0f;
        if(Input::Input::IsKeyPressed(VIPER_KEY_D))
            m_DirSpeedX = 1.0f; //m_CameraPosition.x += 1.0f * ts.deltatime() * 4.0f;
        
        if(Input::Input::IsKeyPressed(VIPER_KEY_A))
            m_DirSpeedX = -1.0f;//m_CameraPosition.x -= 1.0f * ts.deltatime() * 4.0f;

        if(Input::Input::IsKeyPressed(VIPER_KEY_W))
            m_DirSpeedY = 1.0f;//m_CameraPosition.y += 1.0f * ts.deltatime() * 4.0f;
        
        if(Input::Input::IsKeyPressed(VIPER_KEY_S))
            m_DirSpeedY = -1.0f; //m_CameraPosition.y -= 1.0f * ts.deltatime() * 4.0f;

        m_CameraPosition.x += m_DirSpeedX * ts.deltatime() * 4.0f;
        m_CameraPosition.y += m_DirSpeedY * ts.deltatime() * 4.0f;
        RecalculateMatrix();
    };

    void EditorCamera::RecalculateMatrix() {
        m_Projection = glm::perspective(glm::radians(m_CameraFOV), m_AspectRatio, m_CameraNear, m_CameraFar);
    };

    bool EditorCamera::OnMouseScrolled(Events::MouseScrollEvent& event) {
        m_CameraPosition.z -= (float)event.y; // fel sätt att göra det på.
        // använd forward vector istället, och basera CameraPosition på det.
        return false;
    };

    void EditorCamera::OnEvent(Events::Event& event) {
        Events::EventDispatcher dispatcher(event);
        dispatcher.Dispatch< Events::MouseScrollEvent >( VIPER_GET_EVENT_FUNC(EditorCamera::OnMouseScrolled));
    };

    glm::mat4 EditorCamera::GetTransform() {
		glm::mat4 rotation = glm::toMat4(glm::quat(m_CameraRotation));

		return glm::translate(glm::mat4(1.0f), m_CameraPosition) * rotation * glm::scale(glm::mat4(1.0f), m_CameraScale);
	}
};