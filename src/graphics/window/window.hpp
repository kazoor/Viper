#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <glfw/glfw3.h>
#include "../../events/event/event.hpp"
#include "../../events/eventhandler/eventhandler.hpp"

namespace Viper::Graphics {
    struct WindowParams_t {
        int Width;            // Window width.
        int Height;           // Window height.
        std::string Title;    // Window title.
        GLFWmonitor *Monitor; // The monitor to use for full screen mode, or `NULL` for windowed mode.
        GLFWwindow *Share;    // The window whose context to share resources with, or `NULL` to not share resources.
        Events::EventBus *EventCallback;
    };

    struct WindowFrameBufferSizeEvent : public Viper::Events::Event {
        WindowFrameBufferSizeEvent(int Width, int Height) : Width(Width), Height(Height) {}

        int Width, Height;
    };

    struct WindowResizeEvent : public Viper::Events::Event {
        WindowResizeEvent(int Width, int Height) : Height(Height), Width(Width) {}

        int Width, Height;
    };

    struct WindowPositionEvent : public Viper::Events::Event {
        WindowPositionEvent(int X, int Y) : X(X), Y(Y) {}

        int X, Y;
    };

    struct WindowContentScaleEvent : public Viper::Events::Event {
        WindowContentScaleEvent(float XScale, float YScale) : XScale(XScale), YScale(YScale) {}

        float XScale, YScale;
    };

    struct WindowCloseEvent : public Viper::Events::Event {
    };

    class Window {
    public:
        Window(int Width, int Height, const std::string &WindowName);

        ~Window();

        void Initialize();

        void Update() const;

        bool Closed() const;

        static GLFWwindow *CreateWindowEx(WindowParams_t Params);

        void ProcessInput(GLFWwindow *Window);

        inline GLFWwindow *Ctx() { return Context; }

        void OnWindowFrameBufferSizeEvent(WindowFrameBufferSizeEvent *E);

        void OnWindowResizeEvent(WindowResizeEvent *E);

        void OnWindowPositionEvent(WindowPositionEvent *E);

        void OnWindowContentScaleEvent(WindowContentScaleEvent *E);

        void OnWindowCloseEvent(WindowCloseEvent *E);


    private:
        WindowParams_t WindowParams;

        GLFWwindow *Context;

        Events::EventBus *WindowEvents;

        bool isClosed;
    };
}