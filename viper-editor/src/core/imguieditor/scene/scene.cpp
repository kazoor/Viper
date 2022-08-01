#include "scene.hpp"
// utils:
#include <util/globals/global.hpp>
#include <util/timer/timestep.hpp>
#include <util/physics/physics2d.hpp>

// components:
#include <components/camera.hpp>
#include <components/input.hpp>
#include <components/gameobject.hpp>
#include <components/spriterenderer.hpp>

#include <entt/entt.hpp>

struct EnTT_Transform {
    glm::vec3 position;
    EnTT_Transform() = default;
    EnTT_Transform( const EnTT_Transform& ) = default;
    EnTT_Transform( const glm::vec3& pos ) : position( pos ) { };
};

namespace Viper {
    Scene::Scene(void* context) : Layer("Scene"), WindowContext((GLFWwindow*)context) {
        m_Camera = new Renderer::OrthoGraphicCamera(-AspectRatio * 2.0f, AspectRatio * 2.0f, 2.0f, -2.0f, 1.0f, -1.0f);
        Renderer::Renderer2D::Instantiate();
        AspectRatio = 0.0f;

        Globals::ConsoleContext::AddLog( VIPER_ICON_SUCC " Success!", "Window has been loaded!", Globals::ConsoleSuccess );
        
        m_TexSprite = Renderer::Sprite2D::Create("resources/textures/teamspeak.jpg");
        m_TexSprite2 = Renderer::Sprite2D::Create("resources/textures/checkerboard.png");

        entt::entity ent = m_Registry.create(); // Skapar en uint32_t entity id.
        
        // Denna gör samma sak som vi hade gjort med vårt gamla system.
        // object->AddComponent< Transform >( glm::vec3(1.0f, 0.2f, 0.3f ) );
        m_Registry.emplace< EnTT_Transform >( ent, glm::vec3(1.0f, 0.2f, 0.3f ) );

        // Som vårt gamla. Transform& has_component = GetComponent< Transform >( )
        auto& has_component = m_Registry.get< EnTT_Transform >( ent );

        
    };

    Scene::~Scene() {
        Destroy();
    };

    void Scene::Destroy() {
        delete m_Camera;
        Renderer::Renderer2D::Destroy();
    };

    void Scene::OnUpdate(Timestep::Timestep ts) {
        Renderer::Renderer2D::BindFramebuffer();
        Renderer::RenderCommand::Clear();
        Renderer::Renderer2D::UnbindFramebuffer();


        Renderer::Renderer2D::BindFramebuffer();
        Graphics::WindowParams_t &WindowData = *(Graphics::WindowParams_t *)glfwGetWindowUserPointer(WindowContext);

        AspectRatio = ( float )Globals::Editor::SceneW / ( float )Globals::Editor::SceneH;

        Renderer::Renderer2D::Begin(*m_Camera);
        for( int y = -20; y < 20; y++ )
            for( int x = -20; x < 20; x++ )
                Renderer::Renderer2D::DrawQuad(glm::vec2(x, y), ( x + y ) % 2 ? RendererAPI::Color(0.6f, 0.6f, 0.6f) : RendererAPI::Color(0.7f, 0.7f, 0.7f));

        static float posx = 0.0f;
        static float posy = 0.0f;
        static float rad = 0.0f;

        //Renderer::Renderer2D::DrawQuadRotated(glm::vec2(posx, posy), rad * ( 3.141592f / 180.0f ), RendererAPI::Color::Green());

        Renderer::Renderer2D::DrawTexture(glm::vec2(1.0f, 0.0f), m_TexSprite2);
        
        static float rad_ex = 1.0f;

        rad_ex += 1.0f * ts * 32.0f;

        Renderer::Renderer2D::DrawLine(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(15.0f, 15.0f, 0.0f), RendererAPI::Color::Blue());
        
        Renderer::Renderer2D::DrawQuad(glm::vec2(4.0f, 4.0f), RendererAPI::Color(0.2f, 0.2f, 1.0f));

        for(auto& go : *Globals::GlobalsContext::Gom ) {
            go->OnUpdate(ts);

            if( go->HasComponent< Components::SpriteRenderer >( ) ) {
                auto& spr = go->GetComponent< Components::SpriteRenderer >( );
                auto& tr = go->GetComponent< Components::Transform >( );
            };
            
            if( go->HasComponent< Components::Camera >( ) && Globals::Editor::isPlaying ) {
                if(!go->GetComponent< Components::Camera >( ).enabled)
                    continue;
//
                auto camera_position = go->GetComponent< Components::Transform >( ).position;
                auto camera_scale = go->GetComponent< Components::Transform >( ).scale;
//
                m_Camera->SetProjection(-AspectRatio * camera_scale.z, AspectRatio * camera_scale.z, camera_scale.z, -camera_scale.z, 1.0f, -1.0f);
                m_Camera->SetPosition( glm::vec3( camera_position.x, camera_position.y, 0.0f ) );
            };
        };

        if(!Globals::Editor::isPlaying )
            m_Camera->SetProjection(-AspectRatio * Globals::Editor::ZoomLevel, AspectRatio * Globals::Editor::ZoomLevel, Globals::Editor::ZoomLevel, -Globals::Editor::ZoomLevel, 1.0f, -1.0f);

        Renderer::Renderer2D::End();
        Renderer::Renderer2D::UnbindFramebuffer();
    }

    void Scene::OnEvent(Events::Event& event) {
        for(auto& go : *Globals::GlobalsContext::Gom ) {
            go->OnEvent(event);
        };
    };
};