#include "global.hpp"


namespace Viper::Globals {
    Renderer::Renderer2D* Renderer2D = nullptr;
    Components::GameObjectManager* Gom = nullptr;
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

    void CreateGomContext() {
        Gom = new Components::GameObjectManager();
    };
    
    void DestroyGomContext() {
        delete Gom;
    };

    namespace Editor {
        float ZoomLevel = 2.25f;
        float Position[3] = { 0.0f, 0.0f, 0.0f };
        float Radians = 180.f;
        float LightDensity = 1.0f;
        double DeltaTime = 0.0;

        std::vector< Console_t > m_Errors{};
    };
};