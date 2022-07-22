#include "inputhandler.hpp"

namespace Viper::Input {
    Input* Input::Instance = new InputHandler();

    bool InputHandler::IsKeyPressedImpl(int KeyCode) {
        auto State = glfwGetKey(glfwGetCurrentContext(), KeyCode);

        return State == GLFW_PRESS || State == GLFW_REPEAT;
    }

    bool InputHandler::IsMouseKeyPressedImpl(int KeyCode) {
        auto State = glfwGetMouseButton(glfwGetCurrentContext(), KeyCode);

        return State == GLFW_PRESS;
    }

    std::pair<double, double> InputHandler::GetMousePositionImpl() {
        double X, Y;
       glfwGetCursorPos(glfwGetCurrentContext(), &X, &Y);

        return {X, Y};
    }

    double InputHandler::GetMouseXImpl() {
        auto[X, Y] = GetMousePositionImpl();

        return X;
    }

    double InputHandler::GetMouseYImpl() {
        auto[X, Y] = GetMousePositionImpl();

        return Y;
    }

    std::pair<double, double> InputHandler::GetScrollInputImpl() {
        return std::pair<double, double>();
    }
}