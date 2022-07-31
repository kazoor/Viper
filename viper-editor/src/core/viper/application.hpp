#pragma once
#include <util/ref/reference.hpp>
#include <events/event/event.hpp>
#include <graphics/window/window.hpp>
#include <layers/layer/layer.hpp>

namespace Viper {
    class VIPER_API Application {
    public:
        Application();

        ~Application();

        void OnEvent( Events::Event& event );

        void PushLayer(Layers::Layer* layer);

        void PushOverlay(Layers::Layer* overlay);

        void Run();

        virtual void ApplyLayers(void* appcontext) = 0;

        GLFWwindow* GetContext();
    private:
        Scope< Graphics::Window > m_Window;
    };
};