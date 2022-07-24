#pragma once

#include "../../layers/layer/layer.hpp"
#include "../../events/event/event.hpp"
#include "../../graphics/window/window.hpp"
#include "../../util/globals/global.hpp"
#include "../../graphics/renderer/camera/orthographic_camera.hpp"

namespace Viper::Scene {
    class Scene : public Viper::Layers::Layer {
    public:
        Scene(Viper::Graphics::Window* Window) : Layer("Scene"), WindowContext(Window) {
           //gom = new Components::GameObjectManager();
           //auto new_gob = std::make_unique< Components::GameObject >( );
           //         new_gob->AddComponent< Components::Game >( );
           //gom->OnAdd(std::move( new_gob ));
           //gom->OnAwake();

           AspectRatio = 0.0f;
        };

        ~Scene() {
            Destroy();
        };

        void Destroy() {
            
        };

        void OnUpdate() override {
            Viper::Graphics::WindowParams_t &WindowData = *(Viper::Graphics::WindowParams_t *) glfwGetWindowUserPointer(
                    WindowContext->Ctx());

            Globals::GlobalsContext::Gom->OnUpdate();

            auto Ratio = GetCurrentWindowSize( );

            AspectRatio = ( float )Ratio.first / ( float )Ratio.second;
            
            Globals::GlobalsContext::Renderer2D->BindFramebuffer();
            
            static float zoom = 0.0f;
            zoom = Lerp( zoom, Globals::Editor::ZoomLevel, GetDeltaTime() * 4.0f );
            Globals::GlobalsContext::Renderer2D->Begin(Renderer::OrthoGraphicCamera(-AspectRatio * zoom, AspectRatio * zoom, 
                zoom, -zoom, 1.0f, -1.0f));

            Globals::GlobalsContext::Renderer2D->PushVec2("u_LightPos", glm::vec2(1.0f,1.0f));
            Globals::GlobalsContext::Renderer2D->PushFloat("u_LightDensity", Globals::Editor::LightDensity);

            for( int y = -50; y < 30; y++ )
                for( int x = -50; x < 30; x++ )
                    Globals::GlobalsContext::Renderer2D->DrawQuad(glm::vec2(x, y), RendererAPI::Color::White());

            static float posx = 0.0f;
            static float posy = 0.0f;
            static float rad = 0.0f;

            posx = Lerp(posx, Globals::Editor::Position[0], GetDeltaTime() * 3.0f );
            posy = Lerp(posy, Globals::Editor::Position[1], GetDeltaTime() * 3.0f );
            rad = Lerp(rad, Globals::Editor::Radians, GetDeltaTime() * 3.0f );
            
            Globals::GlobalsContext::Renderer2D->DrawQuadRotated(glm::vec2(posx, posy), rad * ( 3.141592f / 180.0f ), RendererAPI::Color::Green());

            for(auto& go : Globals::GlobalsContext::Gom->m_GameObjects ) {
                go->OnUpdate();
                //if( go->HasComponent< Components::Transform >( ) ) {
                //    auto& tr = go->GetComponent< Components::Transform >( );
                //    Globals::GlobalsContext::Renderer2D->DrawQuadRotated(glm::vec2(tr.position.x, tr.position.y), glm::vec2(tr.scale.x, tr.scale.y), tr.rotation.z, RendererAPI::Color::Red());
                //};
            };

            Globals::GlobalsContext::Renderer2D->Flush();
            Globals::GlobalsContext::Renderer2D->End();
            Globals::GlobalsContext::Renderer2D->UnbindFramebuffer();
        }

        std::pair< float, float > GetCurrentWindowSize() const {
            int m_TempWidth, m_TempHeight;
            glfwGetWindowSize(WindowContext->Ctx(), &m_TempWidth, &m_TempHeight);
            return std::make_pair( static_cast< float >( m_TempWidth ), static_cast< float >( m_TempHeight ) );
        }

        void OnEvent(Viper::Events::Event *Event) override {

        }

        double GetDeltaTime() const {
            return Globals::Editor::DeltaTime;
        };

        float Lerp( float a, float b, float t ) {
            return a + ( b - a ) * t;
        };
    private:
        Graphics::Window* WindowContext;
    private:
        float AspectRatio;
    };
}