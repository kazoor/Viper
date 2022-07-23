#include "global.hpp"

namespace Viper::Globals {
    Renderer::Renderer2D* Renderer2D = nullptr;
    Events::EventBus* EventHandler = nullptr;

    void CreateContext() {

    };

    void DestroyContext() {
    };

    void CreateRenderingContext() {
        Renderer2D = new Renderer::Renderer2D();
    };

    void CreateEventHandlerContext() {
        EventHandler = new Events::EventBus();
    };

    void DestroyRenderingContext() {
        delete Renderer2D;
        delete EventHandler;
    };

    void DestroyEventHandlerContext() {
        delete EventHandler;
    };

    namespace Editor {
        float ZoomLevel = 2.25f;
        float Position[3] = { 0.0f, 0.0f, 0.0f };
        float Radians = 1.8f;
    };
};