#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <glfw/glfw3.h>

struct WindowParams
{
    int m_Width;
    int m_Height;
    std::string m_Title;
    GLFWmonitor *m_Monitor; // The monitor to use for full screen mode, or `NULL` for windowed mode.
    GLFWwindow *m_Share;    // The window whose context to share resources with, or `NULL` to not share resources.
};

class Window
{
public:
    Window(int Width, int Height, const std::string &WindowName);
    ~Window();

    void Initialize();
    GLFWwindow *CreateWindow(WindowParams Params);
    void ProcessInput(GLFWwindow *Window);
    static void FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height);

    inline GLFWwindow *Context() { return m_Window; }

private:
    WindowParams m_WindowParams;
    GLFWwindow *m_Window;
};