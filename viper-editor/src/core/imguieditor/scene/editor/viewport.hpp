#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>
#include <scene/scene.hpp>

namespace Viper {
    class SceneViewport {
    public:
        SceneViewport() = default;
        SceneViewport( Scene* SceneContext, void* WindowContext );

        void OnImGuiRender(Timestep::Timestep ts);
        void OnImGuiScene(Timestep::Timestep ts);
        void OnImGuiPlay(Timestep::Timestep ts);
    private:
        Scene* m_Context;
        void* m_WindowContext;
    };
};