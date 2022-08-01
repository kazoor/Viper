#include "sceneentity.hpp"

namespace Viper {
    Entity::Entity( ) : m_handle_entity( entt::null ), m_scene_object_ptr( nullptr ) { };
    Entity::Entity( entt::entity ent, Scene* object ) : m_handle_entity( ent ), m_scene_object_ptr( object ) { };
};