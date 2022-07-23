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

    namespace Editor {
        float ZoomLevel = 2.25f;
        float Position[3] = { 0.0f, 0.0f, 0.0f };
        float Radians = 1.8f;
        float LightDensity = 1.0f;

        std::vector< Console_t > m_Errors{};
    };
};