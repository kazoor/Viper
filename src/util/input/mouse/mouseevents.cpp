#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <utility>
#include "mouseevents.hpp"
#include "../../../graphics/window/window.hpp"
#include "../../globals/global.hpp"

double MousePosX, MousePosY;

namespace Viper::Input {
    MouseEvents::MouseEvents() {
        auto Ctx = glfwGetCurrentContext();

        glfwSetCursorPosCallback(Ctx, [](GLFWwindow *Window, double XPos, double YPos) {
            Viper::Globals::EventHandler->Commit(new MouseCursorPositionEvent(XPos, YPos));
        });
    }

    void MouseEvents::OnMouseCursorPositionEvent(MouseCursorPositionEvent *E) {
        MousePosX = E->XPos;
        MousePosY = E->YPos;
    }

    std::pair<double, double> MouseEvents::GetMousePosition() {
        return {MousePosX, MousePosY};
    }
}