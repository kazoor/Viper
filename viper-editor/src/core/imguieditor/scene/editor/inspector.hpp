#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>
#include <scene/scene.hpp>

namespace Viper {
    class SceneInspector {
    public:
        SceneInspector() = default;
        SceneInspector(Scene* SceneContext );

        void OnImGuiRender( Timestep::Timestep ts);
    private:
        Scene* m_Context;
    };
};