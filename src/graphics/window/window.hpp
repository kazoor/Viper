#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <glfw/glfw3.h>

namespace Viper::Graphics {
    struct WindowParams_t {
        int Width;            // Window width.
        int Height;           // Window height.
        std::string Title;    // Window title.
        GLFWmonitor *Monitor; // The monitor to use for full screen mode, or `NULL` for windowed mode.
        GLFWwindow *Share;    // The window whose context to share resources with, or `NULL` to not share resources.
    };


    class Window {
    public:
        Window(int Width, int Height, const std::string &WindowName);

        ~Window();

        void Initialize();

        void Update() const;

        bool Closed() const;

        static GLFWwindow *CreateWindow(WindowParams_t Params);

        void ProcessInput(GLFWwindow *Window);

        static void FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height);

        inline GLFWwindow *Ctx() { return Context; }

    private:
        WindowParams_t WindowParams;
        GLFWwindow *Context;
        bool isClosed;
    };
}