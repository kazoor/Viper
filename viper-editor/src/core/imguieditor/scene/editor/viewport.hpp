#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>
#include <scene/scene.hpp>

namespace Viper::Editor {
    class SceneViewport {
    public:
        SceneViewport() = default;
        SceneViewport(const Ref< Scene >& SceneContext, void* WindowContext );

        void OnImGuiRender(Timestep::Timestep ts);
        void OnImGuiScene(Timestep::Timestep ts);
        void OnImGuiPlay(Timestep::Timestep ts);
    private:
        Ref< Scene > m_Context;
        void* m_WindowContext;
    };
};