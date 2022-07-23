#pragma once
#include "../../graphics/renderer/renderer.hpp"
#include "../../events/eventhandler/eventhandler.hpp"

namespace Viper::Globals {
    void CreateContext();
    void DestroyContext();

    void CreateRenderingContext();
    void DestroyRenderingContext();

    void CreateEventHandlerContext();
    void DestroyEventHandlerContext();

    extern Renderer::Renderer2D* Renderer2D;
    extern Events::EventBus* EventHandler;

    namespace Editor { // Debugging.
        extern float ZoomLevel;
        extern float Position[3];
        extern float Radians;
    };
};