#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>
#include <scene/scene.hpp>

namespace Viper::Editor {
    class SceneInspector {
    public:
        SceneInspector() = default;
        SceneInspector(const Ref< Scene >& SceneContext );

        void OnImGuiRender( Timestep::Timestep ts);
    private:
        Ref< Scene > m_Context;
    };
};