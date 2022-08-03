// Copyright (c) 2022.

#include <string>
#include <functional>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "window.hpp"
#include <util/input/inputhandler/inputhandler.hpp>
#include <util/input/mouse/mouseevents.hpp>
#include <imguieditor/imguieditor.hpp>
#include <graphics/renderer/rendercommand.hpp>
#include <imguieditor/scene/scenelayer.hpp>
#include <thread>

namespace Viper::Graphics {
    static float m_LastFrame = 0.0f;
    #define VIPER_GET(wnd) WindowParams_t& data = *(WindowParams_t*)glfwGetWindowUserPointer(wnd)
    Window::Window(int Width, int Height, const std::string &WindowName) {
        glfwInit();

        WindowParams = {
                Width,
                Height,
                WindowName,
                nullptr,
                nullptr,
                nullptr};
//
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    };

    Window::~Window() {
        printf("bye bye window.\n");
    };

    static void glfwErrorHandleCallback( int errormode, const char* message ) {
        printf("[GLFW ERROR!] Error Type: %d, Message: %s\n", errormode, message);
    };

    void Window::CreateEvents() {
        glfwSetFramebufferSizeCallback(Context, [](GLFWwindow *Window, int Width, int Height) {
            VIPER_GET(Window); // okay, so the reason why im using the glfw's pointer
            // is because, glfwSet functions doesn't allow you to use "this" for the lambda. So i can't really place the eventcallback
            // in the private section of the class.
            data.Width = Width;
            data.Height = Height;
            Events::WindowFrameBufferSizeEvent event(Width, Height);
            data.EventCallback(event);
        });

        glfwSetWindowSizeCallback(Context, [](GLFWwindow *Window, int Width, int Height) {
            VIPER_GET(Window);
            Events::WindowResizeEvent event(Width, Height);
            data.EventCallback(event);
        });

        glfwSetWindowPosCallback(Context, [](GLFWwindow *Window, int X, int Y) {
            WindowParams_t &param = *(WindowParams_t *) glfwGetWindowUserPointer(Window);
            Events::WindowPositionEvent event(X, Y);
            param.EventCallback(event);
        });

        glfwSetWindowContentScaleCallback(Context, [](GLFWwindow *Window, float XScale, float YScale) {
            VIPER_GET(Window);
            Events::WindowContentScaleEvent event(XScale, YScale);
            data.EventCallback(event);
        });

        glfwSetWindowMaximizeCallback(Context, [](GLFWwindow *Window, int Maximized) {
            VIPER_GET(Window);
            Events::WindowMaximizationEvent event(Maximized);
            data.EventCallback(event);
        });

        glfwSetWindowFocusCallback(Context, [](GLFWwindow *Window, int Focused) {
            VIPER_GET(Window);
            Events::WindowFocusEvent event(Focused);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(Context, [](GLFWwindow *Window) {
            VIPER_GET(Window);
            Events::WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(Context, [](GLFWwindow *Window, double xpos, double ypos) {
            VIPER_GET(Window);
            Events::MouseCursorPositionEvent event((double) xpos, (double) ypos);
            data.EventCallback(event);
        });

        glfwSetScrollCallback(Context, [](GLFWwindow *Window, double xpos, double ypos) {
            VIPER_GET(Window);
            Events::MouseScrollEvent event(xpos, ypos);
            data.EventCallback(event);
        });


        glfwSetKeyCallback(Context, [](GLFWwindow *Window, int Key, int ScanCode, int Action, int Mods) {
            VIPER_GET(Window);

            switch (Action) {
                case GLFW_PRESS: {
                    Events::KeyboardKeyPressedEvent Event(Key, false);
                    data.EventCallback(Event);
                    break;
                }
                case GLFW_REPEAT: {
                    Events::KeyboardKeyPressedEvent Event(Key, true);
                    data.EventCallback(Event);
                    break;
                }
                case GLFW_RELEASE: {
                    Events::KeyboardKeyReleasedEvent Event(Key);
                    data.EventCallback(Event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(Context, [](GLFWwindow *Window, int Button, int Action, int mods) {
            VIPER_GET(Window);
            switch (Action) {
                case GLFW_PRESS: {
                    Events::MouseButtonPressedEvent Event(Button);
                    data.EventCallback(Event);
                    break;
                }
                case GLFW_RELEASE: {
                    Events::MouseButtonReleasedEvent Event(Button);
                    data.EventCallback(Event);
                    break;
                }
            }
        });
    };

    void Window::Update() {
        float m_CurrentFrame = ( float )glfwGetTime();
        Timestep::Timestep ts = m_CurrentFrame - m_LastFrame;
        m_LastFrame = m_CurrentFrame;
        
        for (auto Layer: *LayerStack) {
            Layer->OnUpdate(ts);
        }

        glfwSwapBuffers(Context);
        glfwPollEvents();
    };

    bool Window::IsRunning() {
        return !glfwWindowShouldClose(Context);
    };

    void Window::Setup() {
        Context = CreateWindowEx(WindowParams);
        if (!Context) {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
        }

        glfwMakeContextCurrent(Context);
        glfwSetWindowUserPointer(Context, &WindowParams);
        glfwSetErrorCallback(glfwErrorHandleCallback);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            spdlog::error("Failed to initialize GLAD");
        }
        CreateEvents();
        LayerStack = new Layers::LayerStack();
        glfwSwapInterval(1); // Vsync on for now.
    };

    GLFWwindow* Window::CreateWindowEx(WindowParams_t Params) {
        return glfwCreateWindow(Params.Width, Params.Height, Params.Title.c_str(), Params.Monitor, Params.Share);
    };

    void Window::Shutdown() {
        delete LayerStack;
        glfwDestroyWindow(Context);
        glfwTerminate();
    };

    void Window::PushLayer(Layers::Layer* layer) {
        LayerStack->PushLayer( layer );
    };

    void Window::PushOverlay(Layers::Layer* overlay) {
        LayerStack->PushOverlay( overlay );
    }

    // Event seciont

    void Window::OnEvent(Events::Event& event) {
        {
            Events::EventDispatcher dispatcher(event);

            dispatcher.Dispatch< Events::WindowFrameBufferSizeEvent >( VIPER_GET_EVENT_FUNC( Window::OnWindowFrameBufferSizeEvent ) );
            dispatcher.Dispatch< Events::WindowResizeEvent >( VIPER_GET_EVENT_FUNC( Window::OnWindowResizeEvent ) );
            dispatcher.Dispatch< Events::WindowCloseEvent >( VIPER_GET_EVENT_FUNC( Window::OnWindowCloseEvent ) );
            
            dispatcher.Dispatch<Events::MouseCursorPositionEvent>(
                    VIPER_GET_EVENT_FUNC(Window::OnWindowMouseCursorPositionEvent));
            dispatcher.Dispatch<Events::MouseScrollEvent>(VIPER_GET_EVENT_FUNC(Window::OnWindowMouseScrollEvent));

            dispatcher.Dispatch<Events::KeyboardKeyPressedEvent>(VIPER_GET_EVENT_FUNC(Window::OnKeyboardKeyPressedEvent));

            dispatcher.Dispatch<Events::KeyboardKeyReleasedEvent>(VIPER_GET_EVENT_FUNC(Window::OnKeyboardKeyReleasedEvent));

            dispatcher.Dispatch<Events::MouseButtonPressedEvent>(VIPER_GET_EVENT_FUNC(Window::OnMouseButtonPressedEvent));

            dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(VIPER_GET_EVENT_FUNC(Window::OnMouseButtonReleasedEvent));
        }

        for( auto it = LayerStack->rbegin(); it != LayerStack->rend(); it++ ) {
            if(event.handled)
                break;

            ( *it )->OnEvent( event );
        };
    };

    void Window::RegisterCallback( const fn_EventCallback& callback ) {
        WindowParams.EventCallback = callback;
    };

    bool Window::OnWindowFrameBufferSizeEvent(Events::WindowFrameBufferSizeEvent &E) {
        spdlog::info("WindowFrameBufferSize Event triggered! Updated viewport to: {0}x{1}", E.Width, E.Height);
        glViewport(0, 0, E.Width, E.Height);
        WindowParams.Width = E.Width;
        WindowParams.Height = E.Height;
        RenderCommand::Resize(E.Width, E.Height);
        return true;
    }

    bool Window::OnWindowResizeEvent(Events::WindowResizeEvent &E) {
        spdlog::info("WindowResize Event triggered! New size is {0}x{1}", E.Width, E.Height);
        WindowParams.Width = E.Width;
        WindowParams.Height = E.Height;
        RenderCommand::Resize(E.Width, E.Height);
        return true;
    }

    bool Window::OnWindowPositionEvent(Events::WindowPositionEvent &E) {
        spdlog::info("WindowPosition Event triggered! New position is X: {0} : Y: {1}", E.X, E.Y);
        return true;
    }

    bool Window::OnWindowContentScaleEvent(Events::WindowContentScaleEvent &E) {
        spdlog::info("WindowContentScale Event triggered! Scale (X:Y): {0}:{1}", E.XScale, E.YScale);
        return true;
    }

    bool Window::OnWindowMaximizationEvent(Events::WindowMaximizationEvent &E) {
        spdlog::info("WindowMaximization Event triggered! Value: {0}", E.Maximized);
        return true;
    }

    bool Window::OnWindowFocusEvent(Events::WindowFocusEvent &E) {
        spdlog::info("WindowFocus Event triggered! Focus: {0}", E.Focused);
        return true;
    }

    bool Window::OnWindowCloseEvent(Events::WindowCloseEvent &E) {
        spdlog::info("WindowCloseEvent Event triggered!");
        return true;
    }

    bool Window::OnWindowMouseCursorPositionEvent(Events::MouseCursorPositionEvent &E) {
        VIPER_LOG("MouseCursorPositionEvent Event triggered! {0}, {1}", E.x, E.y);
        Globals::Editor::MousePosX = E.x;
        Globals::Editor::MousePosY = E.y;
        return true;
    };
    
    bool Window::OnWindowMouseScrollEvent(Events::MouseScrollEvent &E) {
        VIPER_LOG("MouseScrollEvent Event triggered! {0}, {1}", E.x, E.y);
        if (!Globals::Editor::isPlaying)
            Globals::Editor::ZoomLevel -= static_cast< float >( E.y );
        return true;
    };

    bool Window::OnKeyboardKeyPressedEvent(Events::KeyboardKeyPressedEvent &E) {
        VIPER_LOG("KeyboardKeyPressed: {0} | IsHeld = {1}", E.Key, E.IsHeld);
        return true;
    }

    bool Window::OnKeyboardKeyReleasedEvent(Events::KeyboardKeyReleasedEvent &E) {
        VIPER_LOG("KeyboardKeyReleased: {0}", E.Key);
        return true;
    }

    bool Window::OnMouseButtonPressedEvent(Events::MouseButtonPressedEvent &E) {
        VIPER_LOG("MouseButtonPressedEvent: {0}", E.Button);
        return true;
    }

    bool Window::OnMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent &E) {
        VIPER_LOG("MouseButtonPressedEvent: {0}", E.Button);
        return true;
    };
};