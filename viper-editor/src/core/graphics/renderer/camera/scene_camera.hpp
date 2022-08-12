#pragma once
#include "camera.hpp"

namespace Viper {
    class SceneCamera : public Camera {
    public:
        enum class CameraTypes : int { Perspective = 0, Orthographic = 1 };
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float verticalFov, float nearClip, float farClip);

        void SetViewportSize(uint32_t width, uint32_t height);

        CameraTypes GetCameraType() const { return type; };

        void SetProjectionType( CameraTypes t ) { type = t; RecalculateMatrix();};

        float GetVerticalFov() const { return m_VerticalFOV; };
        float GetClipNear() const { return m_OrthographicNear; };
        float GetClipFar() const { return m_OrthographicFar; };

        void SetVerticalFov(float fov) { 
            m_VerticalFOV = fov; 
            RecalculateMatrix(); 
        };
        
    private:
        void RecalculateMatrix();
    private:
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f;
        float m_OrthographicFar = 1.0f;
        float m_VerticalFOV = 10.0f;

        float m_AspectRatio = 0.0f;

        CameraTypes type = CameraTypes::Orthographic;
    };
};