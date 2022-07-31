#include <glad/glad.h>
#include "application.hpp"
#include <graphics/renderer/renderer.hpp>
#include <util/globals/global.hpp>

namespace Viper {
    Application::Application() {
        //m_Window = new Graphics::Window(1280, 720, "Viper");
        //m_Window->SetCallback([this](Events::Event& e) { return Application::OnEvent(e);});
        //m_Window->Init();
        
        m_Window = CreateScope< Graphics::Window >( 1280, 720, "Viper" );
        m_Window->Setup();
        m_Window->RegisterCallback([this](Events::Event& e){ return Application::OnEvent(e);});
        printf("created new callback.\n");
    };

    void Application::Run() {
        Globals::GlobalsContext::CreateContext();
        ApplyLayers( GetContext( ) );
        while(m_Window->IsRunning()) {
            m_Window->Update();
        };
    };

    GLFWwindow* Application::GetContext() {
        return m_Window->Ctx();
    };

    Application::~Application() {
        printf("deconstructed.\n");
        Globals::GlobalsContext::DestroyContext();
        m_Window->Shutdown();
    };

    void Application::OnEvent(Events::Event& e) {
        //m_Window->OnEvent(e);
        printf("event: %s\n", e.GetName());
        m_Window->OnEvent(e);
     };

    void Application::PushLayer(Layers::Layer* layer) {
        m_Window->PushLayer( layer );
    };

    void Application::PushOverlay(Layers::Layer* overlay) {
        m_Window->PushLayer( overlay );
    };
};