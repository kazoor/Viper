#include "inputhandler.hpp"

double m_ScrollX = 0.0;
double m_ScrollY = 0.0;

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

    double InputHandler::GetScrollXImpl() {
        auto[X, Y] = GetScrollInputImpl();

        return X;
    }

    double InputHandler::GetScrollYImpl() {
        auto[X, Y] = GetScrollInputImpl();

        return Y;
    }

    void InputHandler::ResetScrollImpl() { // Reset each frame.
        m_ScrollY = 0.0;
        m_ScrollY = 0.0;
    }

    void ScrollCallback(GLFWwindow* wnd, double offsetx, double offsety) {
        m_ScrollX = offsetx;
        m_ScrollY = offsety;
        spdlog::info("Detected scroll: {0}, {1}", offsetx, offsety);
    }

    std::pair<double, double> InputHandler::GetScrollInputImpl() {
        glfwSetScrollCallback(glfwGetCurrentContext(), ScrollCallback);
        return { m_ScrollX, m_ScrollY };
    }
}