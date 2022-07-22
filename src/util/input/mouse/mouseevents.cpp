#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <utility>
#include "mouseevents.hpp"
#include "../../../graphics/window/window.hpp"

double MousePosX, MousePosY;

namespace Viper::Input {
    MouseEvents::MouseEvents() {
        auto Ctx = glfwGetCurrentContext();

        glfwSetCursorPosCallback(Ctx, [](GLFWwindow *Window, double XPos, double YPos) {
            Graphics::WindowParams_t &WindowData = *(Graphics::WindowParams_t *) glfwGetWindowUserPointer(Window);
            WindowData.EventCallback->Commit(new MouseCursorPositionEvent(XPos, YPos));
        });
    }

    void MouseEvents::OnMouseCursorPositionEvent(MouseCursorPositionEvent *E) {
        // spdlog::info("MouseCursorPosition Event triggered! New position is X: {0} : Y: {1}", E->XPos, E->YPos);
        MousePosX = E->XPos;
        MousePosY = E->YPos;
    }

    std::pair<double, double> MouseEvents::GetMousePosition() {
        return {MousePosX, MousePosY};
    }
}