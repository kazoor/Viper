#pragma once

#include "../../layers/layer/layer.hpp"
#include "../../events/event/event.hpp"
#include "../../graphics/window/window.hpp"
#include "../../graphics/renderer/renderer.hpp"
#include "../../graphics/renderer/camera/orthographic_camera.hpp"

namespace Viper::Scene {
    class Scene : public Viper::Layers::Layer {
    public:
        Scene(Viper::Graphics::Window* Window) : Layer("Scene"), WindowContext(Window) {
           renderer = new Renderer::Renderer2D();
           AspectRatio = 0.0f;
           ZoomLevel = 5.0f;
        };

        ~Scene() {
            Destroy();
        };

        void Destroy() {
            delete renderer;
        };

        void OnUpdate() override {
            Viper::Graphics::WindowParams_t &WindowData = *(Viper::Graphics::WindowParams_t *) glfwGetWindowUserPointer(
                    WindowContext->Ctx());

            auto Ratio = GetCurrentWindowSize( );

            AspectRatio = ( float )Ratio.first / ( float )Ratio.second;
            
            renderer->Begin(Renderer::OrthoGraphicCamera(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, ZoomLevel, -ZoomLevel, 1.0f, -1.0f));

            for( int y = -10; y < 10; y++ )
                for( int x = -10; x < 10; x++ )
                    renderer->DrawQuad(glm::vec2(x, y), RendererAPI::Color(x * 50, 100 - ( y * 20 ), 255 - (x * 3), 255));

            renderer->DrawQuad(glm::vec2(0.25f, 0.76f), RendererAPI::Color::Green());

            renderer->Flush();
            
            renderer->End();
        }

        std::pair< float, float > GetCurrentWindowSize() const {
            int m_TempWidth, m_TempHeight;
            glfwGetWindowSize(WindowContext->Ctx(), &m_TempWidth, &m_TempHeight);
            return std::make_pair( static_cast< float >( m_TempWidth ), static_cast< float >( m_TempHeight ) );
        }

        void OnEvent(Viper::Events::Event *Event) override {

        }
    private:
        Renderer::Renderer2D* renderer;
        Viper::Graphics::Window* WindowContext;
    private:
        float AspectRatio;
        float ZoomLevel;
    };
}