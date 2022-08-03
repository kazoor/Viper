#include "sceneentity.hpp"

namespace Viper {
    Entity::Entity( ) : m_handle_entity( entt::null ), m_scene_object_ptr( nullptr ) { };
    Entity::Entity( entt::entity ent, Scene* object ) : m_handle_entity( ent ), m_scene_object_ptr( object ) { };
    Entity::Entity( const Entity& other ) : m_handle_entity( other.m_handle_entity ), m_scene_object_ptr( other.m_scene_object_ptr ) { };
};