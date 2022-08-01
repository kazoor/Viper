#pragma once
#include <util/timer/timestep.hpp>

namespace Viper::Editor {
    class SceneFilexplorer {
    public:
        SceneFilexplorer() = default;

        void OnImGuiRender( Timestep::Timestep ts );
        void OnImGuiConsole( Timestep::Timestep ts );
    };
};