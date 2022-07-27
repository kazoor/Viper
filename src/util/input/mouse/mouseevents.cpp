#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <utility>
#include "mouseevents.hpp"

double MousePosX, MousePosY;

namespace Viper::Input {
    // TODO: Fix events here
    MouseEvents::MouseEvents() {
        auto Ctx = glfwGetCurrentContext();
        glfwSetCursorPosCallback(Ctx, [](GLFWwindow *Window, double XPos, double YPos) {
            MousePosX = XPos;
            MousePosY = YPos;
        });
    }

    std::pair<double, double> MouseEvents::GetMousePosition() {
        return {MousePosX, MousePosY};
    }
}