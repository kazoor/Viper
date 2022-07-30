// Copyright (c) 2022.

#include <string>
#include <functional>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "window.hpp"
#include "../../util/input/inputhandler/inputhandler.hpp"
#include "../../imguieditor/imguieditor.hpp"
#include "../../imguieditor/scene/scene.hpp"
#include "../../util/input/mouse/mouseevents.hpp"
#include <thread>

namespace Viper::Graphics {
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

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void Window::ProcessInput(GLFWwindow *Window) {
        // if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // glfwSetWindowShouldClose(Window, true);
    }

    void Window::Init() {
        Context = CreateWindowEx(WindowParams);
        if (!Context) {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
        }

        glfwMakeContextCurrent(Context);
        glfwSetWindowUserPointer(Context, &WindowParams);
        glfwSwapInterval(1); // Vsync on for now.

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            spdlog::error("Failed to initialize GLAD");
        }

        LayerStack = new Layers::LayerStack();
        Globals::GlobalsContext::CreateContext();

        UpdateWindowEvents();

        PushLayer(new ImGuiEditor(this));
        PushLayer(new Scene::Scene(this));
        //PushLayer(new Input::KeyboardInputLayer());
        //PushLayer(new Input::MouseInputLayer());

        static double previous_delta = glfwGetTime();
        while (!glfwWindowShouldClose(Context)) {
            ProcessInput(Context);

            double current_delta = glfwGetTime();
            Globals::Editor::DeltaTime = current_delta - previous_delta;
            previous_delta = current_delta;

            Renderer::Renderer2D::BindFramebuffer();
            glClear(GL_COLOR_BUFFER_BIT);
            Renderer::Renderer2D::UnbindFramebuffer();

            for (auto Layer: *LayerStack) {
                //spdlog::info("Updating Layer {0}", Layer->GetLayerName());
                Layer->OnUpdate();
            }

            Update();
        }

        Globals::GlobalsContext::DestroyContext();
        delete LayerStack;

        glfwDestroyWindow(Context);
        glfwTerminate();
    };

    void Window::OnEvent(Events::Event &event) {
        // Kommer att säga detta en gång, och en gång bara.
        // Vi dispatchar dem ENBART för att logga.
        // Dem blir redan calla'de av EventCallback.
        {
            Events::EventDispatcher dispatcher(event);

            dispatcher.Dispatch< WindowFrameBufferSizeEvent >( VIPER_GET_EVENT_FUNC( Window::OnWindowFrameBufferSizeEvent ) );
            dispatcher.Dispatch< WindowResizeEvent >( VIPER_GET_EVENT_FUNC( Window::OnWindowResizeEvent ) );
            dispatcher.Dispatch< WindowCloseEvent >( VIPER_GET_EVENT_FUNC( Window::OnWindowCloseEvent ) );
            
            dispatcher.Dispatch<MouseCursorPositionEvent>(
                    VIPER_GET_EVENT_FUNC(Window::OnWindowMouseCursorPositionEvent));
            dispatcher.Dispatch<MouseScrollEvent>(VIPER_GET_EVENT_FUNC(Window::OnWindowMouseScrollEvent));

            dispatcher.Dispatch<KeyboardKeyPressedEvent>(VIPER_GET_EVENT_FUNC(Window::OnKeyboardKeyPressedEvent));

            dispatcher.Dispatch<KeyboardKeyReleasedEvent>(VIPER_GET_EVENT_FUNC(Window::OnKeyboardKeyReleasedEvent));

            dispatcher.Dispatch<MouseButtonPressedEvent>(VIPER_GET_EVENT_FUNC(Window::OnMouseButtonPressedEvent));

            dispatcher.Dispatch<MouseButtonReleasedEvent>(VIPER_GET_EVENT_FUNC(Window::OnMouseButtonReleasedEvent));

            dispatcher.Dispatch<WindowFocusEvent>(VIPER_GET_EVENT_FUNC(Window::OnWindowFocusEvent));
        }

        for( auto it = LayerStack->rbegin(); it != LayerStack->rend(); it++ ) {
            if(event.handled)
                break;

            ( *it )->OnEvent( event );
        };
    };

    void Window::SetCallback(const fn_EventCallback &callback) {
        WindowParams.EventCallback = callback;
    };

    void Window::UpdateWindowEvents() {
        glfwSetFramebufferSizeCallback(Context, [](GLFWwindow *Window, int Width, int Height) {
            VIPER_GET(Window); // okay, so the reason why im using the glfw's pointer
            // is because, glfwSet functions doesn't allow you to use "this" for the lambda. So i can't really place the eventcallback
            // in the private section of the class.
            data.Width = Width;
            data.Height = Height;
            WindowFrameBufferSizeEvent event(Width, Height);
            data.EventCallback(event);
        });

        glfwSetWindowSizeCallback(Context, [](GLFWwindow *Window, int Width, int Height) {
            VIPER_GET(Window);
            WindowResizeEvent event(Width, Height);
            data.EventCallback(event);
        });

        glfwSetWindowPosCallback(Context, [](GLFWwindow *Window, int X, int Y) {
            WindowParams_t &param = *(WindowParams_t *) glfwGetWindowUserPointer(Window);
            WindowPositionEvent event(X, Y);
            param.EventCallback(event);
        });

        glfwSetWindowContentScaleCallback(Context, [](GLFWwindow *Window, float XScale, float YScale) {
            VIPER_GET(Window);
            WindowContentScaleEvent event(XScale, YScale);
            data.EventCallback(event);
        });

        glfwSetWindowMaximizeCallback(Context, [](GLFWwindow *Window, int Maximized) {
            VIPER_GET(Window);
            WindowMaximizationEvent event(Maximized);
            data.EventCallback(event);
        });

        glfwSetWindowFocusCallback(Context, [](GLFWwindow *Window, int Focused) {
            VIPER_GET(Window);
            WindowFocusEvent event(Focused);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(Context, [](GLFWwindow *Window) {
            VIPER_GET(Window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(Context, [](GLFWwindow *Window, double xpos, double ypos) {
            VIPER_GET(Window);
            MouseCursorPositionEvent event((double)xpos, (double) ypos);
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
                    KeyboardKeyPressedEvent Event(Key, false);
                    data.EventCallback(Event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyboardKeyPressedEvent Event(Key, true);
                    data.EventCallback(Event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyboardKeyReleasedEvent Event(Key);
                    data.EventCallback(Event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(Context, [](GLFWwindow *Window, int Button, int Action, int mods) {
            VIPER_GET(Window);
            switch (Action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent Event(Button);
                    data.EventCallback(Event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent Event(Button);
                    data.EventCallback(Event);
                    break;
                }
            }
        });

        //bool OnWindowKeyEvent(KeyboardKeyPressedEvent& E);
    }

    void Window::Update() const {
        glfwSwapBuffers(Context);
        glfwPollEvents();
        //Input::Input::ResetScroll();
    }

    void Window::PushLayer(Layers::Layer *Layer) {
        LayerStack->PushLayer(Layer);
    }

    void Window::PushOverlay(Layers::Layer *Overlay) {
        LayerStack->PushOverlay(Overlay);
    }

    bool Window::Closed() const {
        return glfwWindowShouldClose(Context);
    }

    GLFWwindow *Window::CreateWindowEx(WindowParams_t Params) {
        return glfwCreateWindow(Params.Width, Params.Height, Params.Title.c_str(), Params.Monitor, Params.Share);
    }

    bool Window::OnWindowFrameBufferSizeEvent(WindowFrameBufferSizeEvent &E) {
        spdlog::info("WindowFrameBufferSize Event triggered! Updated viewport to: {0}x{1}", E.Width, E.Height);
        glViewport(0, 0, E.Width, E.Height);
        WindowParams.Width = E.Width;
        WindowParams.Height = E.Height;
        Renderer::Renderer2D::ResizeFBO(E.Width, E.Height);
        return true;
    }

    bool Window::OnWindowResizeEvent(WindowResizeEvent &E) {
        spdlog::info("WindowResize Event triggered! New size is {0}x{1}", E.Width, E.Height);
        WindowParams.Width = E.Width;
        WindowParams.Height = E.Height;
        Renderer::Renderer2D::ResizeFBO(E.Width, E.Height);
        return true;
    }

    bool Window::OnWindowPositionEvent(WindowPositionEvent &E) {
        spdlog::info("WindowPosition Event triggered! New position is X: {0} : Y: {1}", E.X, E.Y);
        return true;
    }

    bool Window::OnWindowContentScaleEvent(WindowContentScaleEvent &E) {
        spdlog::info("WindowContentScale Event triggered! Scale (X:Y): {0}:{1}", E.XScale, E.YScale);
        return true;
    }

    bool Window::OnWindowMaximizationEvent(WindowMaximizationEvent &E) {
       spdlog::info("WindowMaximization Event triggered! Value: {0}", E.Maximized);
        return true;
    }

    bool Window::OnWindowFocusEvent(WindowFocusEvent &E) {
        spdlog::info("WindowFocus Event triggered! Focus: {0}", E.Focused);
        return true;
    }

    bool Window::OnWindowCloseEvent(WindowCloseEvent &E) {
        spdlog::info("WindowCloseEvent Event triggered!");
        return true;
    }

    bool Window::OnWindowMouseCursorPositionEvent(MouseCursorPositionEvent &E) {
        VIPER_LOG("MouseCursorPositionEvent Event triggered! {0}, {1}", E.x, E.y);
        Globals::Editor::PosX = E.x;
        Globals::Editor::PosY = E.y;
        return true;
    };
    
    bool Window::OnWindowMouseScrollEvent(MouseScrollEvent &E) {
        VIPER_LOG("MouseScrollEvent Event triggered! {0}, {1}", E.x, E.y);
        if (!Globals::Editor::isPlaying)
            Globals::Editor::ZoomLevel -= static_cast< float >( E.y );
        return true;
    };

    bool Window::OnKeyboardKeyPressedEvent(KeyboardKeyPressedEvent &E) {
        VIPER_LOG("KeyboardKeyPressed: {0} | IsHeld = {1}", E.Key, E.IsHeld);
        return true;
    }

    bool Window::OnKeyboardKeyReleasedEvent(KeyboardKeyReleasedEvent &E) {
        VIPER_LOG("KeyboardKeyReleased: {0}", E.Key);
        return true;
    }

    bool Window::OnMouseButtonPressedEvent(MouseButtonPressedEvent &E) {
        VIPER_LOG("MouseButtonPressedEvent: {0}", E.Button);
        return true;
    }

    bool Window::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &E) {
        VIPER_LOG("MouseButtonPressedEvent: {0}", E.Button);
        return true;
    };
}