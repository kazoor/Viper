#pragma once
#include "camera.hpp"

namespace Viper {
    class SceneCamera : public Camera {
    public:
        enum class ProjectionType : int { Perspective = 0, Orthographic = 1 };
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic( float size, float near, float far );

        void SetViewportSize( uint32_t width, uint32_t height );

        const ProjectionType& GetProjectionType() const { return m_ProjectionType; };

        void SetProjectionType( ProjectionType type ) { m_ProjectionType = type; };

        float GetOrthographicSize() const { return m_OrthoSize; };
        float GetOrthographicNear() const { return m_OrthoNear; };
        float GetOrthographicFar() const { return m_OrthoFar; };

        void SetOrthographicSize(float ortho_size) { m_OrthoSize = ortho_size; Recalculate();};
        void SetOrthographicNear(float ortho_near) { m_OrthoNear = ortho_near; Recalculate();};
        void SetOrthographicFar(float ortho_far) { m_OrthoFar = ortho_far; Recalculate();};
    private:
        void Recalculate();
    private:
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;
        float m_OrthoSize = 10.0f;
        float m_OrthoNear = -1.0f;
        float m_OrthoFar = 1.0f;
        float m_AspectRatio = 0.0f;
    };
};