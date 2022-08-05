#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>

namespace Viper {
    class Scene;
    class SceneHierarchy {
    public:
        SceneHierarchy() = default;
        SceneHierarchy( Scene* SceneContext );

        void OnImGuiRender( Timestep::Timestep ts );
        void OnImGuiRenderItems( Timestep::Timestep ts );
    private:
        Scene* m_Context;
        Scene* m_Editor;
    };
};