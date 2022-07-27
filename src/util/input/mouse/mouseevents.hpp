#pragma once
#include <utility>
#include <string>
#include <sstream>

namespace Viper::Input {

    class MouseEvents {
    public:
        MouseEvents();

        static std::pair<double, double> GetMousePosition();
    };
}