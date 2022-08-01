#pragma once
#include <util/timer/timestep.hpp>
#include <entt/entt.hpp>

namespace Viper {

    class Entity;
    class Scene {
    public:
        Entity CreateEntity();
        
        void DestroyEntity( entt::entity ent );
        
        void OnUpdate(Timestep::Timestep ts);
    private:
        friend class Entity;
        entt::registry m_register;
    };
};