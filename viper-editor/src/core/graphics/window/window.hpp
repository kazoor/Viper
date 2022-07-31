#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include "../../external/glfw/include/GLFW/glfw3.h"
#include "../../events/event/event.hpp"
#include "../../util/input/mouse/mouseevents.hpp"
#include "../../layers/layer/layer.hpp" // <!-- BASE INCLUDED.
#include "../../layers/layerstack/layerstack.hpp"
#include "../../viper/viper.hpp"

namespace Viper::Graphics {
    using fn_EventCallback = std::function< void(Events::Event&) >;
    struct VIPER_API WindowParams_t {
        int Width;            // Window width.
        int Height;           // Window height.
        std::string Title;    // Window title.
        GLFWmonitor *Monitor; // The monitor to use for full screen mode, or `NULL` for windowed mode.
        GLFWwindow *Share;    // The window whose context to share resources with, or `NULL` to not share resources.

        fn_EventCallback EventCallback;
    };
    class VIPER_API Window {
    public:
        Window(int Width, int Height, const std::string &WindowName);
        ~Window();

        void Setup();
        void Shutdown();

        void PushLayer( Layers::Layer* layer);
        void PushOverlay( Layers::Layer* overlay);

        void Update();

        bool IsRunning();
        void OnEvent(Events::Event& event);
        void CreateEvents();
        void RegisterCallback( const fn_EventCallback& callback );

        GLFWwindow* CreateWindowEx(WindowParams_t Params);
        WindowParams_t WindowParams;
        GLFWwindow* Ctx() { return Context; };
    public: // event section

        // Sends an event when the framebuffer of the window is resized.
        bool OnWindowFrameBufferSizeEvent(Events::WindowFrameBufferSizeEvent& E);

        // Sends an event when the window is resized.
        bool OnWindowResizeEvent(Events::WindowResizeEvent& E);

        // Sends an event when the position of the window is changed.
        bool OnWindowPositionEvent(Events::WindowPositionEvent& E);

        // Sends an event when the content scale of the window is changed.
        bool OnWindowContentScaleEvent(Events::WindowContentScaleEvent& E);

        // Sends an event when the window is maximized or restored.
        bool OnWindowMaximizationEvent(Events::WindowMaximizationEvent& E);

        // Sends an event when the window loses/gains input focus.
        bool OnWindowFocusEvent(Events::WindowFocusEvent& E);

        // Sends an event when window is closing.
        bool OnWindowCloseEvent(Events::WindowCloseEvent& E);

        // Sends an event everytime the mouse position has CHANGED.
        bool OnWindowMouseCursorPositionEvent(Events::MouseCursorPositionEvent& E);

        bool OnWindowMouseScrollEvent(Events::MouseScrollEvent& E);

        bool OnKeyboardKeyPressedEvent(Events::KeyboardKeyPressedEvent& E);

        bool OnKeyboardKeyReleasedEvent(Events::KeyboardKeyReleasedEvent& E);

        bool OnMouseButtonPressedEvent(Events::MouseButtonPressedEvent& E);

        bool OnMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& E);
    private:
        GLFWwindow* Context;

        Layers::LayerStack* LayerStack;
    };
};