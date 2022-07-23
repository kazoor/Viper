#include "global.hpp"


namespace Viper::Globals {
    Renderer::Renderer2D* Renderer2D = nullptr;
    void CreateContext() {

    };

    void DestroyContext() {
    };

    void CreateRenderingContext() {
        Renderer2D = new Renderer::Renderer2D();
    };

    void DestroyRenderingContext() {
        delete Renderer2D;
    };
};