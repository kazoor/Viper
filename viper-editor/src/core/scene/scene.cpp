// vendor items
#include <glm/vec3.hpp>

#include <graphics/renderer/renderer2d.hpp>

#include "scene.hpp"
#include "sceneentity.hpp"
#include "entitycomponents.hpp"

namespace Viper {
    inline static void OnTransformConstruct( entt::registry& reg, entt::entity entity_handle ) {
        printf("call construct.\n"); 
    };

    static b2BodyType Rigidbody2DTypeToBox2D(Rigidbody2DComponent::BodyType bodytype) {
        switch( bodytype ) {
            case Rigidbody2DComponent::BodyType::body_static: return b2_staticBody;
            case Rigidbody2DComponent::BodyType::body_dynamic: return b2_dynamicBody;
            case Rigidbody2DComponent::BodyType::body_kinematic: return b2_kinematicBody;
        };

        printf("[viper] err: unknown body type.\n");
        return b2_staticBody;
    };

    Scene::Scene() {
    };

    Scene::~Scene() {
    };

    template< typename... Components >
    Entity Scene::GetView( ) {
        return m_register.view< Components... >( );
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
        const int velocity_iterations = 6;
        const int position_iterations = 2;
        
        if( m_box_world ) {
            m_box_world->Step(ts, velocity_iterations, position_iterations);

            auto view = m_register.view< Rigidbody2DComponent >( );
            for( auto entity : view ) {
                printf("box_world valid\n");
                Entity ent = { entity, this };
                auto& transform = ent.get< TransformComponent >( );
                auto& rb2d = ent.get< Rigidbody2DComponent >( );

                b2Body* body = ( b2Body* )rb2d.Rigidbody;
                const auto& pos = body->GetPosition();

                transform.position.x = pos.x;
                transform.position.y = pos.y;
                transform.rotation.z = body->GetAngle();
            };
        };
        // Sprite & Transform
        {
            auto view = m_register.view< TransformComponent, SpriteRendererComponent >( );
            for( auto entity : view ) {
                auto [tr, spr] = view.get< TransformComponent, SpriteRendererComponent >( entity );
                if( spr.sprite.get( ) ) {
                    Renderer2D::DrawRotatedTexture( { tr.position.x, tr.position.y }, { tr.scale.x, tr.scale.y }, spr.sprite, spr.tiling, tr.rotation.z, glm::vec4(spr.color.x, spr.color.y, spr.color.z, spr.color.w) );
                } else Renderer2D::DrawQuadRotated( { tr.position.x, tr.position.y }, { tr.scale.x, tr.scale.y }, tr.rotation.z, spr.color);
            };
        }
    };

    void Scene::OnPhysics() {
        
    };

    void Scene::ResetViewport( ) {
        m_selected_entity = entt::null;  
    };

    void Scene::OnPhysicsStart() {
        m_box_world = new b2World({0.0f, 9.8f});

        auto view = m_register.view< Rigidbody2DComponent >( );
        for( auto entity : view ) {
            Entity ent = { entity, this };
            auto& transform = ent.get< TransformComponent >( );
            auto& rb2d = ent.get< Rigidbody2DComponent >( );

            b2BodyDef body;
            body.type = Rigidbody2DTypeToBox2D( rb2d.Type );//(b2BodyType)rb2d.Type;
            body.position.Set(transform.position.x, transform.position.y);
            body.fixedRotation = rb2d.FixedRotation;
            body.angle = transform.rotation.z;

            auto body_body = m_box_world->CreateBody(&body);
            body_body->SetFixedRotation(rb2d.FixedRotation);
            rb2d.Rigidbody = body_body;

            if( ent.has< BoxCollider2DComponent >( ) ) {
                auto& bc2d = ent.get< BoxCollider2DComponent >( );

                b2PolygonShape polygonShanpe;
                polygonShanpe.SetAsBox(
                        bc2d.size.x * transform.scale.x, 
                        bc2d.size.y * transform.scale.y
                    );

                b2FixtureDef fixture;
                fixture.shape = &polygonShanpe;
                fixture.density = bc2d.density;
                fixture.friction = bc2d.friction;
                fixture.restitution = bc2d.restitution;
                fixture.restitutionThreshold = bc2d.restitutionthreshold;
                body_body->CreateFixture(&fixture);
            };
        };
    };

    void Scene::OnPhysicsEnd() {
        delete m_box_world;
        m_box_world = nullptr;
    };

    void Scene::OnPhysicsUpdate() {

    };
};