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
        ent.add< TransformComponent >( );
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

                transform.Translation.x = pos.x;
                transform.Translation.y = pos.y;
                transform.Translation.z = body->GetAngle();
            };
        };
        Camera* m_MainCamera = nullptr;
        glm::mat4 m_MainTransform;
        {
            auto view = m_register.view< CameraComponent, TransformComponent >( );
            for( auto camera_entity : view ) {
                auto [cam, trans] = view.get< CameraComponent, TransformComponent >( camera_entity );
                if( cam.MainCamera ) {
                    m_MainCamera = &cam.camera;
                    m_MainTransform = trans.GetTransform();
                    break;
                };
            }; 
        };
        if( m_MainCamera ) {
            Renderer2D::Begin( m_MainCamera->GetProjection(), m_MainTransform );

            auto group = m_register.group< TransformComponent >( entt::get< SpriteRendererComponent > );
            for( auto entity : group ) {
                auto [tr, spr] = group.get< TransformComponent, SpriteRendererComponent >( entity );

                Renderer2D::DrawSprite(tr.GetTransform(), spr);
            };
            Renderer2D::End();
        }
    };

    void Scene::OnOverlay( Timestep::Timestep ts ) {
        auto group = m_register.group< TransformComponent >( entt::get< BoxCollider2DComponent > );
        for( auto entity : group ) {
            auto [tr, spr] = group.get< TransformComponent, BoxCollider2DComponent >( entity );
            Renderer2D::DrawOutlinedQuad( { tr.Translation.x, tr.Translation.y }, { tr.Scale.x, tr.Scale.y }, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        };
    };

    void Scene::OnPhysics() {
        
    };

    Entity Scene::GetSelectedEntity() {
        return Entity{ m_selected_entity, this };
    };

    void Scene::ResetViewport( ) {
        m_selected_entity = entt::null;  
    };

    void Scene::OnPhysicsStart() {
        m_box_world = new b2World({0.0f, -9.8f});

        auto view = m_register.view< Rigidbody2DComponent >( );
        for( auto entity : view ) {
            Entity ent = { entity, this };
            auto& transform = ent.get< TransformComponent >( );
            auto& rb2d = ent.get< Rigidbody2DComponent >( );

            b2BodyDef body;
            body.type = Rigidbody2DTypeToBox2D( rb2d.Type );//(b2BodyType)rb2d.Type;
            body.position.Set(transform.Translation.x, transform.Translation.y);
            body.fixedRotation = rb2d.FixedRotation;
            body.angle = transform.Rotation.z;
            body.gravityScale = rb2d.Gravity;

            auto body_body = m_box_world->CreateBody(&body);
            body_body->SetFixedRotation(rb2d.FixedRotation);
            rb2d.Rigidbody = body_body;

            if( ent.has< BoxCollider2DComponent >( ) ) {
                auto& bc2d = ent.get< BoxCollider2DComponent >( );

                b2PolygonShape polygonShanpe;
                polygonShanpe.SetAsBox(
                        bc2d.size.x * transform.Scale.x, 
                        bc2d.size.y * transform.Scale.y
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

    Entity Scene::GetCameraEntity() {
        auto view = m_register.view< CameraComponent >( );
        for( auto entity : view ) {
            auto& camera = view.get< CameraComponent >( entity );
            if( camera.MainCamera )
                return Entity{ entity, this };
        };
        return {};
    };

    void Scene::OnPhysicsUpdate() {

    };

    void Scene::OnViewportResize( uint32_t width, uint32_t height ) {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        
        auto view = m_register.view< CameraComponent >( );
        for( auto entity : view ) {
            auto& cameraComponent = view.get< CameraComponent >( entity );
            if(!cameraComponent.FixedAspectRatio) {
                cameraComponent.camera.SetViewportSize( width, height );  
            };
        };
    };
};