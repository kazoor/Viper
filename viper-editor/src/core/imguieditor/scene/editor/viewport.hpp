#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>
#include <ImGui/imgui.h>
#include <scene/scene.hpp>

namespace Viper {
    class SceneViewport {
    public:
        SceneViewport() = default;
        SceneViewport( Scene* SceneContext, void* WindowContext );

        void OnImGuiRender(Timestep::Timestep ts);
        void OnImGuiScene(Timestep::Timestep ts);
        void OnImGuiPlay(Timestep::Timestep ts);
        void OnImGuiShader(Timestep::Timestep ts);
    private:
        Scene* m_Context;
        void* m_WindowContext;
        ImVec2 ViewportSize{0.0f,0.0f};
    };
};