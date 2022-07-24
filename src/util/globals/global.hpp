#pragma once
#include <string>
#include <vector>
#include "../../graphics/renderer/renderer.hpp"
#include "../../components/gom.hpp"
#include "../../events/eventhandler/eventhandler.hpp"

namespace Viper::Globals {
    class GlobalsContext {
    public:
        static void CreateContext();
        static void DestroyContext();

        static Renderer::Renderer2D* Renderer2D;
        static Components::GameObjectManager* Gom;
        static Events::EventBus* EventHandler;
    };

    struct Console_t {
        std::string StringType;
        std::string ConsoleMessage;
    };

    class ConsoleContext {
    public:
        static void AddLog( const std::string& title, const std::string& log );
        static std::vector< Console_t > GetLogs();
    private:
        static std::vector< Console_t > m_errors;
    };

    namespace Editor { // Debugging.
        extern float ZoomLevel;
        extern float Position[3];
        extern float Radians;
        extern float LightDensity;
        extern double DeltaTime;
        extern int SelectedObject;
    };
};