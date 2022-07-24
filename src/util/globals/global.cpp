#include "global.hpp"
#define VIPER_DECLARE_CONTEXT_NULL( Class, Var ) \
Class GlobalsContext::Var = nullptr; 

namespace Viper::Globals {
    namespace Editor {
        float ZoomLevel = 2.25f;
        float Position[3] = { 0.0f, 0.0f, 0.0f };
        float Radians = 180.f;
        float LightDensity = 1.0f;
        double DeltaTime = 0.0;
        int SelectedObject = -1;
    };

    // == // == // == // == // == // == //

    VIPER_DECLARE_CONTEXT_NULL(Renderer::Renderer2D*, Renderer2D)
    VIPER_DECLARE_CONTEXT_NULL(Components::GameObjectManager*, Gom)
    VIPER_DECLARE_CONTEXT_NULL(Events::EventBus*, EventHandler)

    void GlobalsContext::CreateContext() {
        Renderer2D = new Renderer::Renderer2D();
        Gom = new Components::GameObjectManager();
        EventHandler = new Events::EventBus();
    };

    void GlobalsContext::DestroyContext() {
        delete Renderer2D;
        delete Gom;
        delete EventHandler;
    };

    // == // == // == // == // == // == //

    std::vector< Console_t > ConsoleContext::m_errors{};

    void ConsoleContext::AddLog( const std::string& title, const std::string& log ) {
        m_errors.push_back({title, log});
    };

    std::vector< Console_t > ConsoleContext::GetLogs() {
        return m_errors;
    };
};