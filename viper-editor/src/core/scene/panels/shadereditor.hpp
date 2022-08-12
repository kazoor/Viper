#pragma once
#include <util/timer/timestep.hpp>
#include <vector>
#include <string>

namespace Viper {
    class SceneShaderEditor {
    public:
        SceneShaderEditor() = default;

        void OnImGuiRender( Timestep::Timestep ts );
    };
};