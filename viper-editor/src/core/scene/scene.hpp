#pragma once
#include <util/timer/timestep.hpp>
#include <string.h>

#include <imguieditor/scene/editor/hierarchy.hpp>
#include <imguieditor/scene/editor/inspector.hpp>

#include <entt/entt.hpp>

namespace Viper {
    class Entity;
    class Scene {
    public:
        Entity CreateEntity( const std::string& entity_name = "tag_component" );

        void DestroyEntity( entt::entity ent );
        
        void OnUpdate( Timestep::Timestep ts );

        void ResetViewport( );
    private:
        friend class Entity;
        friend class SceneHierarchy;
        friend class SceneInspector;
    private:
        entt::registry m_register;
        entt::entity m_selected_entity = entt::null;
    };
};