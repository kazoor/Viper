#pragma once

// layers:
#include <layers/layer/layer.hpp>

// events:
#include <events/event/event.hpp>

// graphics:
#include <graphics/window/window.hpp>
#include <graphics/renderer/sprite2d.hpp>
#include <graphics/renderer/camera/orthographic_camera.hpp>

// utils:
#include <util/globals/global.hpp>
#include "../../components/input.hpp"

namespace Viper::Scene {
    class Scene : public Layers::Layer {
    public:
        Scene(Viper::Graphics::Window* Window) : Layer("Scene"), WindowContext(Window) {
            m_Camera = new Renderer::OrthoGraphicCamera(-AspectRatio * 2.0f, AspectRatio * 2.0f, 2.0f, -2.0f, 1.0f, -1.0f);
            Renderer::Renderer2D::Instantiate();
            AspectRatio = 0.0f;

            Globals::ConsoleContext::AddLog( VIPER_ICON_SUCC " Success!", "Window has been loaded!", Globals::ConsoleSuccess );
        };

        ~Scene() {
            Destroy();
        };

        void Destroy() {
            delete m_Camera;
            Renderer::Renderer2D::Destroy();
        };

        void OnUpdate() override {
            Graphics::Window::WindowParams_t &WindowData = *(Graphics::Window::WindowParams_t *)glfwGetWindowUserPointer(WindowContext->Ctx());

            AspectRatio = ( float )WindowData.Width / ( float )WindowData.Height;

            Renderer::Renderer2D::Begin(*m_Camera);

            for( int y = -20; y < 20; y++ )
                for( int x = -20; x < 20; x++ )
                    Renderer::Renderer2D::DrawQuad(glm::vec2(x, y), ( x + y ) % 2 ? RendererAPI::Color(0.6f, 0.6f, 0.6f) : RendererAPI::Color(0.7f, 0.7f, 0.7f));

            static float posx = 0.0f;
            static float posy = 0.0f;
            static float rad = 0.0f;

            posx = Lerp(posx, Globals::Editor::Position[0], GetDeltaTime() * 3.0f );
            posy = Lerp(posy, Globals::Editor::Position[1], GetDeltaTime() * 3.0f );
            rad = Lerp(rad, Globals::Editor::Radians, GetDeltaTime() * 3.0f );

            Renderer::Renderer2D::DrawQuadRotated(glm::vec2(posx, posy), rad * ( 3.141592f / 180.0f ), RendererAPI::Color::Green());

            Renderer::Renderer2D::DrawTexture(glm::vec2(1.0f, 0.0f));

            for(auto& go : *Globals::GlobalsContext::Gom ) {
                go->OnUpdate(GetDeltaTime());
                
                if( go->HasComponent< Components::Camera >( ) && Globals::Editor::isPlaying ) {
                    if(!go->GetComponent< Components::Camera >( ).enabled)
                        continue;

                    auto camera_position = go->GetComponent< Components::Transform >( ).position;
                    auto camera_scale = go->GetComponent< Components::Transform >( ).scale;

                    m_Camera->SetProjection(-AspectRatio * camera_scale.z, AspectRatio * camera_scale.z, camera_scale.z, -camera_scale.z, 1.0f, -1.0f);
                    m_Camera->SetPosition( glm::vec3( camera_position.x, camera_position.y, 0.0f ) );
                };
            };

            if(!Globals::Editor::isPlaying )
                m_Camera->SetProjection(-AspectRatio * Globals::Editor::ZoomLevel, AspectRatio * Globals::Editor::ZoomLevel, Globals::Editor::ZoomLevel, -Globals::Editor::ZoomLevel, 1.0f, -1.0f);

            Renderer::Renderer2D::End();
        }

        void OnEvent(Events::Event& event) override {
            for(auto& go : *Globals::GlobalsContext::Gom ) {
                go->OnEvent(event);
            };
        };

        double GetDeltaTime() const {
            return Globals::Editor::DeltaTime;
        };

        float Lerp( float a, float b, float t ) {
            return a + ( b - a ) * t;
        };
    private:
        Graphics::Window* WindowContext;
        Renderer::OrthoGraphicCamera* m_Camera;
        
    private:
        float AspectRatio;
    };
}