#pragma once

#include "../../layers/layer/layer.hpp"
#include "../../events/event/event.hpp"
#include "../../graphics/window/window.hpp"
#include "../../graphics/renderer/renderer.hpp"
#include "../../graphics/shaders/shader/shader.hpp"
#include <glm/gtc/matrix_transform.hpp> // ortho

namespace Viper::Scene {
    class Scene : public Viper::Layers::Layer {
    public:
        Scene(Viper::Graphics::Window* Window) : Layer("Scene"), WindowContext(Window) {
           renderer = new Renderer::Renderer2D();
           shading = new Graphics::Shader("resources/test.vert", "resources/test.frag");
           AspectRatio = 0.0f;
           ZoomLevel = 5.0f;
        };

        ~Scene() {
            Destroy();
        };

        void Destroy() {
            delete renderer;
            delete shading;
        };

        void OnUpdate() override {
            Viper::Graphics::WindowParams_t &WindowData = *(Viper::Graphics::WindowParams_t *) glfwGetWindowUserPointer(
                    WindowContext->Ctx());
            
            renderer->Begin();
            
            for( int y = -10; y < 10; y++ )
                for( int x = -10; x < 10; x++ )
                    renderer->DrawQuad(glm::vec2(x, y), RendererAPI::Color(x * 50, 100 - ( y * 20 ), 255 - (x * 3), 255));
                    
            renderer->Flush();
            auto Ratio = GetCurrentWindowSize( );

            AspectRatio = ( float )Ratio.first / ( float )Ratio.second;
            auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            auto m_ViewMatrix = glm::mat4(1.0f);

            auto m_ProjectionMatrix = glm::ortho(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, ZoomLevel, -ZoomLevel, 1.0f, -1.0f);
            auto m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

            shading->Use();
            shading->SetUniformMat4("u_Transform", m_Transform);
            shading->SetUniformMat4("u_ViewProjection", m_ViewProjectionMatrix);
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
        Graphics::Shader* shading;
        Viper::Graphics::Window* WindowContext;
    private:
        float AspectRatio;
        float ZoomLevel;
    };
}