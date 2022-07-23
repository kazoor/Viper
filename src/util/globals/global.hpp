#pragma once
#include <string>
#include <vector>
#include "../../graphics/renderer/renderer.hpp"

namespace Viper::Globals {
    void CreateContext();
    void DestroyContext();

    void CreateRenderingContext();
    void DestroyRenderingContext();

    extern Renderer::Renderer2D* Renderer2D;

    struct Console_t {
        std::string StringType;
        std::string ConsoleMessage;
    };

    namespace Editor { // Debugging.
        extern float ZoomLevel;
        extern float Position[3];
        extern float Radians;
        extern float LightDensity;

        extern std::vector< Console_t > m_Errors;
    };
};