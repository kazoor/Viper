#pragma once

#include "../../layers/layer/layer.hpp"
#include "../../events/event/event.hpp"
#include "../../graphics/window/window.hpp"
#include "../../util/globals/global.hpp"
#include "../../graphics/renderer/camera/orthographic_camera.hpp"
#include "../../components/demo/gom.hpp"
#include "../../components/game.hpp"

namespace Viper::Scene {
    class Scene : public Viper::Layers::Layer {
    public:
        Scene(Viper::Graphics::Window* Window) : Layer("Scene"), WindowContext(Window) {
           Globals::CreateRenderingContext();

           gom = new Components::GameObjectManager();
           auto new_gob = std::make_unique< Components::GameObject >( );
                    new_gob->AddComponent< Components::Game >( );
           gom->OnAdd(std::move( new_gob ));
           gom->OnAwake();

           AspectRatio = 0.0f;
        };

        ~Scene() {
            Destroy();
        };

        void Destroy() {
            Globals::DestroyRenderingContext();
            delete gom;
        };

        void OnUpdate() override {
            Viper::Graphics::WindowParams_t &WindowData = *(Viper::Graphics::WindowParams_t *) glfwGetWindowUserPointer(
                    WindowContext->Ctx());

            gom->OnUpdate();

            auto Ratio = GetCurrentWindowSize( );

            AspectRatio = ( float )Ratio.first / ( float )Ratio.second;
            
            Globals::Renderer2D->BindFramebuffer();
            
            ImGuiIO& io = ImGui::GetIO();
            static float zoom = 0.0f;
            zoom = Lerp( zoom, Globals::Editor::ZoomLevel, io.DeltaTime * 4.0f );
            Globals::Renderer2D->Begin(Renderer::OrthoGraphicCamera(-AspectRatio * zoom, AspectRatio * zoom, 
                zoom, -zoom, 1.0f, -1.0f));

            Globals::Renderer2D->PushVec2("u_LightPos", glm::vec2(1.0f,1.0f));
            Globals::Renderer2D->PushFloat("u_LightDensity", Globals::Editor::LightDensity);

            for( int y = -10; y < 10; y++ )
                for( int x = -10; x < 10; x++ )
                    Globals::Renderer2D->DrawQuad(glm::vec2(x, y), RendererAPI::Color(x * 50, 100 - ( y * 20 ), 255 - (x * 3), 255));

            static float posx = 0.0f;
            static float posy = 0.0f;
            static float rad = 0.0f;

            posx = Lerp(posx, Globals::Editor::Position[0], io.DeltaTime * 3.0f );
            posy = Lerp(posy, Globals::Editor::Position[1], io.DeltaTime * 3.0f );
            rad = Lerp(rad, Globals::Editor::Radians, io.DeltaTime * 3.0f );
            
            Globals::Renderer2D->DrawQuadRotated(glm::vec2(posx, posy), rad * ( 3.141592f/180.0f ), RendererAPI::Color::Green());

            Globals::Renderer2D->Flush();
            
            Globals::Renderer2D->End();

            Globals::Renderer2D->UnbindFramebuffer();
        }

        std::pair< float, float > GetCurrentWindowSize() const {
            int m_TempWidth, m_TempHeight;
            glfwGetWindowSize(WindowContext->Ctx(), &m_TempWidth, &m_TempHeight);
            return std::make_pair( static_cast< float >( m_TempWidth ), static_cast< float >( m_TempHeight ) );
        }

        void OnEvent(Viper::Events::Event *Event) override {

        }

        float Lerp( float a, float b, float t ) {
            return a + ( b - a ) * t;
        };
    private:
        Graphics::Window* WindowContext;
        Components::GameObjectManager* gom;
    private:
        float AspectRatio;
    };
}