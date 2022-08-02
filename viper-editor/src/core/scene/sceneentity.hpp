#pragma once
#include <cstdint>
#include "scene.hpp"
#include <entt/entt.hpp>

namespace Viper {
    class Entity {
    public:
        Entity();
        Entity( entt::entity ent, Scene* scene );
        Entity( const Entity& other );

        template< typename tty, typename... targs >
        void add( targs&&... args ) {
            m_scene_object_ptr->m_register.emplace< tty >( m_handle_entity, std::forward< targs >( args )... );  
        };

        template< typename tty >
        void remove( ) {
            m_scene_object_ptr->m_register.remove< tty >( m_handle_entity );
        };

        template< typename tty >
        tty& get() {
            return m_scene_object_ptr->m_register.get< tty >( m_handle_entity );
        };

        template< typename tty >
        auto has() {
            return m_scene_object_ptr->m_register.all_of< tty >( m_handle_entity );
        };

        operator entt::entity() const { return m_handle_entity; };
        operator std::uint32_t() const { return static_cast< std::uint32_t >( m_handle_entity ); };
        operator bool() const { return m_handle_entity != entt::null; };
        bool operator==( const Entity& other ) const { return m_handle_entity == other.m_handle_entity && m_scene_object_ptr == other.m_scene_object_ptr; };
    private:
        entt::entity m_handle_entity;
        Scene* m_scene_object_ptr;
    };
};