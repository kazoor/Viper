#include "scene.hpp"
#include "sceneentity.hpp"
#include <glm/vec3.hpp>
#include "entitycomponents.hpp"

namespace Viper {
    inline static void OnTransformConstruct( entt::registry& reg, entt::entity entity_handle ) {
        printf("call construct.\n"); 
    };

    Entity Scene::CreateEntity( const std::string& tag_name ) {
        Entity ent = { m_register.create(), this };
        ent.add< tag_t >( tag_name );
        ent.add< transform_t >( );
        return ent;
    };

    void Scene::DestroyEntity( entt::entity ent ) {
        m_register.destroy( ent );
    };

    void Scene::OnUpdate(Timestep::Timestep ts) {
        auto view = m_register.view< transform_t, tag_t >( );

        for( auto entity : view ) {
            auto [t, tr] = view.get< transform_t, tag_t >( entity );

            const auto& name = tr.tag.c_str( );
            printf("name: %s :: %.2f %.2f %.2f\n", name, t.position.x, t.position.y, t.position.z );
        };
    };

    void Scene::ResetViewport( ) {
        m_selected_entity = entt::null;  
    };
};