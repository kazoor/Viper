#pragma once
#include "scene.hpp"
#include <entt/entt.hpp>

namespace Viper {
    class Entity {
    public:
        Entity();
        Entity( entt::entity ent, Scene* scene );

        template< typename tty, typename... targs >
        void Add( targs&&... args ) {
            m_scene_object_ptr->m_register.emplace< tty >( m_handle_entity, std::forward< targs >( args )... );  
        };

        template< typename tty >
        void Remove( ) {
            m_scene_object_ptr->m_register.remove< tty >( m_handle_entity );
        };

        template< typename tty >
        tty& Get() {
            return m_scene_object_ptr->m_register.get< tty >( m_handle_entity );
        };

        template< typename tty >
        bool& Has() {
            return m_scene_object_ptr->m_register.all_of< tty >( m_handle_entity );
        };

        operator entt::entity() const { return m_handle_entity; };
    private:
        entt::entity m_handle_entity;
        Scene* m_scene_object_ptr;
    };
};