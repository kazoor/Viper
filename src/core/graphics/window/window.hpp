#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <glfw/glfw3.h>
#include "../../events/event/event.hpp"
#include "../../util/input/mouse/mouseevents.hpp"
#include "../../layers/layer/layer.hpp"
#include "../../layers/layerstack/layerstack.hpp"

namespace Viper::Graphics {
    class WindowFrameBufferSizeEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowFrameBufferSize, WindowFrameBufferSizeEvent);
        WindowFrameBufferSizeEvent(int Width, int Height) : Width(Width), Height(Height) { }
        int Width, Height;
    };

    class WindowResizeEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowResize, WindowResizeEvent);
        WindowResizeEvent(int Width, int Height) : Height(Height), Width(Width) { }

        int Width, Height;
    };

    class WindowPositionEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowPosition, WindowPositionEvent);
        WindowPositionEvent(int X, int Y) : X(X), Y(Y) { }

        int X, Y;
    };

    class WindowContentScaleEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowContentScale, WindowContentScaleEvent);
        WindowContentScaleEvent(float XScale, float YScale) : XScale(XScale), YScale(YScale) { }

        float XScale, YScale;
    };

    class WindowMaximizationEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowMaximization, WindowMaximizationEvent);
        WindowMaximizationEvent(int Maximized) : Maximized(Maximized) { }

        int Maximized;
    };

    class WindowFocusEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowFocus, WindowFocusEvent);
        WindowFocusEvent(int Focused) : Focused(Focused) { }

        int Focused;
    };

    class WindowCloseEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowClose, WindowCloseEvent);
    };

    class MouseCursorPositionEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(MouseCursorPosition, MouseCursorPositionEvent);
        MouseCursorPositionEvent( double xpos, double ypos ) : x( xpos ), y( ypos ) { };
        double x, y;
    };

    class Window {
    public:
        struct WindowParams_t {
            int Width;            // Window width.
            int Height;           // Window height.
            std::string Title;    // Window title.
            GLFWmonitor *Monitor; // The monitor to use for full screen mode, or `NULL` for windowed mode.
            GLFWwindow *Share;    // The window whose context to share resources with, or `NULL` to not share resources.

            using fn_EventCallback = std::function< void(Events::Event&) >;
            fn_EventCallback EventCallback;
        };
        WindowParams_t WindowParams;
    public:
        Window(int Width, int Height, const std::string &WindowName);

        ~Window();

        void Init();

        using fn_EventCallback = std::function< void(Events::Event&) >;

        void Initialize();

        void Update() const;

        void PushLayer(Layers::Layer *Layer);

        void PushOverlay(Layers::Layer *Overlay);

        bool Closed() const;

        static GLFWwindow *CreateWindowEx(WindowParams_t Params);

        void ProcessInput(GLFWwindow *Window);

        void UpdateWindowEvents();

        void SetCallback( const fn_EventCallback& callback );

        void OnEvent(Events::Event& event);

        inline GLFWwindow *Ctx() { return Context; }

        // Sends an event when the framebuffer of the window is resized.
        bool OnWindowFrameBufferSizeEvent(WindowFrameBufferSizeEvent& E);

        // Sends an event when the window is resized.
        bool OnWindowResizeEvent(WindowResizeEvent& E);

        // Sends an event when the position of the window is changed.
        bool OnWindowPositionEvent(WindowPositionEvent& E);

        // Sends an event when the content scale of the window is changed.
        bool OnWindowContentScaleEvent(WindowContentScaleEvent& E);

        // Sends an event when the window is maximized or restored.
        bool OnWindowMaximizationEvent(WindowMaximizationEvent& E);

        // Sends an event when the window loses/gains input focus.
        bool OnWindowFocusEvent(WindowFocusEvent& E);

        // Sends an event when window is closing.
        bool OnWindowCloseEvent(WindowCloseEvent& E);

        bool OnWindowMouseCursorPositionEvent(MouseCursorPositionEvent& E);
        
    private:

        GLFWwindow *Context;

        Layers::LayerStack *LayerStack;

        bool isClosed;
    };
}