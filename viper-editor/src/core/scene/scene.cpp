#include "scene.hpp"
#include "sceneentity.hpp"
#include <glm/vec3.hpp>

namespace Viper {
    struct transform_t {
        transform_t() = default;
        transform_t( const transform_t& o ) = default;
        transform_t( const glm::vec3& position ) : pos( position ) { };
        glm::vec3 pos;
    };

    struct tag_t {
        tag_t() = default;
        tag_t( const tag_t& o ) = default;
        tag_t( const std::string& tagname ) : tag( tagname ) { };

        std::string tag;
    };

    inline static void OnTransformConstruct( entt::registry& reg, entt::entity entity_handle ) {
        printf("call construct.\n"); 
    };

    Entity Scene::CreateEntity() {
        Entity ent = { m_register.create(), this };
        ent.Add< transform_t >( );
        ent.Add< tag_t >( "test_component" );
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
            printf("name: %s\n", name );
        };
    };
};