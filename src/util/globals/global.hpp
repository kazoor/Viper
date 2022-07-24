#pragma once
#include <string>
#include <vector>
#include "../../graphics/renderer/renderer.hpp"
#include "../../components/gom.hpp"

namespace Viper::Globals {
    void CreateContext();
    void DestroyContext();

    void CreateRenderingContext();
    void DestroyRenderingContext();

    void CreateGomContext();
    void DestroyGomContext();

    extern Renderer::Renderer2D* Renderer2D;
    extern Components::GameObjectManager* Gom;

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