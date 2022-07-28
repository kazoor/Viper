#include "application.hpp"
#include "window.hpp"
#include "../../viper/base.hpp"

namespace Viper {
    Application::Application() {
        m_Window = new Graphics::Window(1280, 720, "Viper");
        m_Window->SetCallback([this](Events::Event& e) { return Application::OnEvent(e);});
        m_Window->Init();
    };

    void Application::OnEvent(Events::Event& e) {
        m_Window->OnEvent(e);
        VIPER_LOG("call for event :: {0}", e.GetName());
     };
};