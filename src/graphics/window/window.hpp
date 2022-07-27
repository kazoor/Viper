#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <glfw/glfw3.h>
#include <functional>
#include "../../util/input/mouse/mouseevents.hpp"
#include "../../layers/layer/layer.hpp"
#include "../../layers/layerstack/layerstack.hpp"
#include "../../events/event.hpp"

namespace Viper::Graphics {
    // TODO: Fix events here

    class WindowFrameBufferSizeEvent : public Viper::Event {
    public:
        WindowFrameBufferSizeEvent(int Width, int Height) : Width(Width), Height(Height) {}

        int GetWidth() const { return Width; }
        int GetHeight() const { return Height; }


        EVENT_CLASS_TYPE(WindowFrameBufferSize)
        EVENT_CLASS_CATEGORY(WindowEvent)
    private:
        int Width, Height;
    };

    /* struct WindowFrameBufferSizeEvent : public Viper::Events::Event {
        WindowFrameBufferSizeEvent(int Width, int Height) : Width(Width), Height(Height) {}

        int Width, Height;
    };*/

    struct WindowResizeEvent : public Viper::Event {
        WindowResizeEvent(int Width, int Height) : Height(Height), Width(Width) {}

        int Width, Height;
    };

    /*struct WindowPositionEvent : public Viper::Events::Event {
        WindowPositionEvent(int X, int Y) : X(X), Y(Y) {}

        int X, Y;
    };

    struct WindowContentScaleEvent : public Viper::Events::Event {
        WindowContentScaleEvent(float XScale, float YScale) : XScale(XScale), YScale(YScale) {}

        float XScale, YScale;
    };

    struct WindowMaximizationEvent : public Viper::Events::Event {
        WindowMaximizationEvent(int Maximized) : Maximized(Maximized) {}

        int Maximized;
    };

    struct WindowFocusEvent : public Viper::Events::Event {
        WindowFocusEvent(int Focused) : Focused(Focused) {}

        int Focused;
    };

    struct WindowCloseEvent : public Viper::Events::Event { };*/

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;
        struct WindowParams_t {
            int Width;            // Window width.
            int Height;           // Window height.
            std::string Title;    // Window title.
            GLFWmonitor *Monitor; // The monitor to use for full screen mode, or `NULL` for windowed mode.
            GLFWwindow *Share;    // The window whose context to share resources with, or `NULL` to not share resources.
            EventCallbackFn EventCallback;
        };

        Window(int Width, int Height, const std::string &WindowName);

        ~Window();

        void Initialize();

        void Update() const;

        void PushLayer(Layers::Layer *Layer);

        void PushOverlay(Layers::Layer *Overlay);

        bool Closed() const;

        static GLFWwindow *CreateWindowEx(WindowParams_t Params);

        void ProcessInput(GLFWwindow *Window);

        void SetEventSubscriptions();

        void UpdateWindowEvents();

        inline GLFWwindow *Ctx() { return Context; }

        // Sends an event when the framebuffer of the window is resized.
        void OnWindowFrameBufferSizeEvent(WindowFrameBufferSizeEvent *E);

       // Sends an event when the window is resized.
        void OnWindowResizeEvent(WindowResizeEvent *E);

        /*// Sends an event when the position of the window is changed.
        void OnWindowPositionEvent(WindowPositionEvent *E);

        // Sends an event when the content scale of the window is changed.
        void OnWindowContentScaleEvent(WindowContentScaleEvent *E);

        // Sends an event when the window is maximized or restored.
        void OnWindowMaximizationEvent(WindowMaximizationEvent *E);

        // Sends an event when the window loses/gains input focus.
        void OnWindowFocusEvent(WindowFocusEvent *E);

        // Sends an event when window is closing.
        void OnWindowCloseEvent(WindowCloseEvent *E);*/

    private:

        WindowParams_t WindowParams;

        GLFWwindow *Context;

        Layers::LayerStack *LayerStack;

        bool isClosed;
    };
}