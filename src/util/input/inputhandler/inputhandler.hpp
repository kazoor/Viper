#pragma once

#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "../input.hpp"

namespace Viper::Input {
    class InputHandler : public Input {
    protected:
        bool IsKeyPressedImpl(int KeyCode) override;

        bool IsMouseKeyPressedImpl(int KeyCode) override;

        std::pair<double, double> GetMousePositionImpl() override;

        double GetMouseXImpl() override;

        double GetMouseYImpl() override;

        std::pair<double, double> GetScrollInputImpl() override;
    };
}