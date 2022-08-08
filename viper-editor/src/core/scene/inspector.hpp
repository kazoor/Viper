#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>
#include <scene/scene.hpp>

namespace Viper {
    class Entity;
    class SceneInspector {
    public:
        SceneInspector() = default;
        SceneInspector(Scene* SceneContext );

        void OnImGuiRender( Timestep::Timestep ts);
        void OnImGuiPopulateComponents( Entity entity );

        template< typename T >
        void OnImGuiPopulateContext( Entity ent, const std::string& name );
    private:
        Scene* m_Context;
    };
};