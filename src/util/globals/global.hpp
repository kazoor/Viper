#pragma once
#include <string>
#include <vector>
#include "../../graphics/renderer/renderer.hpp"
#include "../../components/gom.hpp"
#include "../../events/eventhandler/eventhandler.hpp"

namespace Viper::Globals {
    void CreateContext();
    void DestroyContext();

    void CreateRenderingContext();
    void DestroyRenderingContext();

    void CreateGomContext();
    void DestroyGomContext();

    extern Components::GameObjectManager* Gom;
    extern Renderer::Renderer2D* Renderer2D;
    extern Events::EventBus* EventHandler;
    
    void CreateEventHandlerContext();
    void DestroyEventHandlerContext();

    struct Console_t {
        std::string StringType;
        std::string ConsoleMessage;
    };

    namespace Editor { // Debugging.
        extern float ZoomLevel;
        extern float Position[3];
        extern float Radians;
        extern float LightDensity;
        extern double DeltaTime;
        extern std::vector< Console_t > m_Errors;
    };
};