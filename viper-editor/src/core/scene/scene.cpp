#include "scene.hpp"
#include "sceneentity.hpp"
#include <glm/vec3.hpp>
#include "entitycomponents.hpp"
#include <graphics/renderer/renderer.hpp>

namespace Viper {
    using namespace Renderer;
    using namespace RendererAPI;
    inline static void OnTransformConstruct( entt::registry& reg, entt::entity entity_handle ) {
        printf("call construct.\n"); 
    };

    Entity Scene::CreateEntity( const std::string& tag_name ) {
        Entity ent = { m_register.create(), this };
        ent.add< TagComponent >( tag_name );
        ent.add< TransformComponent >( 
                glm::vec3( 1.0f, 1.0f, 0.0f ),
                glm::vec3( 1.0f, 1.0f, 0.0f ),
                glm::vec3( 0.0f, 0.0f, 0.0f ) );
        return ent;
    };

    void Scene::DestroyEntity( entt::entity ent ) {
        m_register.destroy( ent );
    };

    void Scene::OnUpdate(Timestep::Timestep ts) {
        auto view = m_register.view< TransformComponent, SpriteRendererComponent >( );

        for( auto entity : view ) {
            auto [tr, spr] = view.get< TransformComponent, SpriteRendererComponent >( entity );

            //printf("name: %.2f %.2f %.2f\n", t.position.x, t.position.y, t.position.z );
            if( spr.sprite_texture.get( ) ) {
                Renderer2D::DrawTexture( { tr.position.x, tr.position.y }, { tr.scale.x, tr.scale.y }, spr.sprite_texture, Color(spr.color.x, spr.color.y, spr.color.z, spr.color.w), spr.tiling );
            }
            else
                Renderer2D::DrawQuad( { tr.position.x, tr.position.y }, { tr.scale.x, tr.scale.y }, Color(spr.color.x, spr.color.y, spr.color.z, spr.color.w));
        };
    };

    void Scene::OnPhysics() {
        //auto view = m_register.view< TransformComponent, Rigidbody2DComponent >( );
        //for( auto entity : view ) {
        //    auto [tr, rg] = view.get< TransformComponent, Rigidbody2DComponent >( entity );
        //};
    };

    void Scene::ResetViewport( ) {
        m_selected_entity = entt::null;  
    };
};