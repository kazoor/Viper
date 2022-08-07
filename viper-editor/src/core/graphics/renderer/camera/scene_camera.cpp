#include "scene_camera.hpp"
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp> // ortho

namespace Viper {
    SceneCamera::SceneCamera() {
        RecalculateMatrix();
    };

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;

        RecalculateMatrix();
    };

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
        m_AspectRatio = (float)width / (float)height;
        RecalculateMatrix();

        //printf("[SceneCamera] Viewport update: %dx%d, aspect: %.2f\n", width, height, (float)width / (float)height);
    };

    void SceneCamera::RecalculateMatrix() {
        float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoBottom = -m_OrthographicSize * 0.5f;
        float orthoTop = m_OrthographicSize * 0.5f;

        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    };
};