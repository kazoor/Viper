// vendor items
#include <glm/vec3.hpp>

#include <graphics/renderer/renderer2d.hpp>
#include <graphics/renderer/renderer3d.hpp>
#include <util/timer/timer.hpp>

#include "scene.hpp"
#include "sceneentity.hpp"
#include "entitycomponents.hpp"

namespace Viper {
    //Mesh* m_Cube = nullptr;

    static b2BodyType Rigidbody2DTypeToBox2D(Rigidbody2DComponent::BodyType bodytype) {
        switch( bodytype ) {
            case Rigidbody2DComponent::BodyType::body_static: return b2_staticBody;
            case Rigidbody2DComponent::BodyType::body_dynamic: return b2_dynamicBody;
            case Rigidbody2DComponent::BodyType::body_kinematic: return b2_kinematicBody;
        };

        printf("[viper] err: unknown body type.\n");
        return b2_staticBody;
    };

    class RaycastCallback : public b2RayCastCallback {
    public:
        RaycastCallback() {
            m_fixture = nullptr;
        };

        float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) {
            m_fixture = fixture;
            m_point = point;
            m_normal = normal;
            m_fraction = fraction;
            return fraction;
        };

    private:
        b2Fixture* m_fixture;
        b2Vec2 m_point;
        b2Vec2 m_normal;
        float m_fraction;
    };

    Scene::Scene() {
        //m_Cube = LoadMeshFromPath("bil.obj");
    };

    Scene::~Scene() {
        //FreeMesh(m_Cube);
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

    void Scene::OnSetEditorTransform( const glm::mat4& camera_projection, const glm::mat4& camera_transform ) {
        default_transform = camera_transform;
        default_projection = camera_projection;
    };

    void Scene::OnUpdate(Timestep::Timestep ts) {
        const int velocity_iterations = 6;
        const int position_iterations = 2;

        if( m_box_world ) {
            m_box_world->Step(ts, velocity_iterations, position_iterations);

            m_box_world->DebugDraw();

            auto view = m_register.view< Rigidbody2DComponent >( );
            for( auto entity : view ) {
                Entity ent = { entity, this };
                auto& transform = ent.get< TransformComponent >( );
                auto& rb2d = ent.get< Rigidbody2DComponent >( );

                b2Body* body = ( b2Body* )rb2d.Rigidbody;
                const auto& pos = body->GetPosition();

                transform.Translation.x = pos.x;
                transform.Translation.y = pos.y;
                transform.Rotation.z = body->GetAngle();
            };
        };
        //Camera* m_MainCamera = nullptr;
        //glm::mat4 m_MainTransform;
        //{
        //    auto view = m_register.view< CameraComponent, TransformComponent >( );
        //    for( auto camera_entity : view ) {
        //        auto [cam, trans] = view.get< CameraComponent, TransformComponent >( camera_entity );
        //        if( cam.MainCamera ) {
        //            m_MainCamera = &cam.camera;
        //            m_MainTransform = trans.GetTransform();
        //            break;
        //        };
        //    }; 
        //};
        //if( m_MainCamera != nullptr ) {
            //{
                    
        {
            Renderer3D::Begin( default_projection, default_transform );
            //Renderer3D::SetLightPosition(light_position, light_color, light_intensity);

                uint32_t m_Index = 0;
                auto group = m_register.group< TransformComponent >( entt::get< MeshComponent > );
                for( auto entity : group ) {
                    auto [tr, msh] = group.get< TransformComponent, MeshComponent >( entity );

                    if(msh.Type == MeshComponent::MeshType::Mesh_Cube )
                        Renderer3D::Quad(tr.GetTransform(), msh.color );
                    else if(msh.Type == MeshComponent::MeshType::Mesh_Light )
                        Renderer3D::SetLightPosition(tr.GetTransform(), tr.Translation, msh.color, 1.0f );//DrawSprite(tr.GetTransform(), spr);
                };
            
            Renderer3D::End();
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
        return Entity{ entt::null, this };
    };

    void Scene::CreateCameraEntity() {
        Entity ent = CreateEntity("MainSceneCamera");
        ent.add< CameraComponent >( );
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