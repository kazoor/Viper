// Copyright (c) 2022.

#include <string>
#include <functional>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "window.hpp"
#include "../../util/input/inputhandler/inputhandler.hpp"
#include "../../imguieditor/imguieditor.hpp"
#include "../../imguieditor/scene/scene.hpp"

namespace Viper::Graphics {

    Window::Window(int Width, int Height, const std::string &WindowName) {
        glfwInit();

        WindowParams = {
                Width,
                Height,
                WindowName,
                nullptr,
                nullptr};

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        Context = CreateWindowEx(WindowParams);
        if (!Context) {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
        }

        glfwMakeContextCurrent(Context);
        glfwSwapInterval(1); // Vsync on for now.

        glfwSetWindowUserPointer(Context, &WindowParams);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            spdlog::error("Failed to initialize GLAD");
        }

        LayerStack = new Viper::Layers::LayerStack();
        Globals::GlobalsContext::CreateContext();

        SetEventSubscriptions();
        UpdateWindowEvents();

        PushLayer(new Viper::ImGuiEditor(this));
        PushLayer(new Viper::Scene::Scene(this));
        PushLayer(new Viper::Input::KeyboardInputLayer());
        PushLayer(new Viper::Input::MouseInputLayer());

        static double previous_delta = glfwGetTime();
        while (!glfwWindowShouldClose(Context)) {
            ProcessInput(Context);

            double current_delta = glfwGetTime();
            Globals::Editor::DeltaTime = current_delta - previous_delta;
            previous_delta = current_delta;

            Globals::GlobalsContext::Renderer2D->BindFramebuffer();
            glClear(GL_COLOR_BUFFER_BIT);
            Globals::GlobalsContext::Renderer2D->UnbindFramebuffer();

            for (auto Layer: *LayerStack) {
                spdlog::info("Updating Layer {0}", Layer->GetLayerName());
                Layer->OnUpdate();
            }

            Update();
        }

        Globals::GlobalsContext::DestroyContext();
        delete LayerStack;

        glfwDestroyWindow(Context);
        glfwTerminate();
    }

    void Window::ProcessInput(GLFWwindow *Window) {
        // if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // glfwSetWindowShouldClose(Window, true);
    }

    void Window::SetEventSubscriptions() {
        // WindowEvents subscriptions.
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Window::OnWindowFrameBufferSizeEvent);
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Window::OnWindowResizeEvent);
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Window::OnWindowPositionEvent);
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Window::OnWindowContentScaleEvent);
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Window::OnWindowMaximizationEvent);
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Window::OnWindowFocusEvent);
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Window::OnWindowCloseEvent);
        Globals::GlobalsContext::EventHandler->Subscribe(new Viper::Input::MouseEvents(),
                                         &Input::MouseEvents::OnMouseCursorPositionEvent);

        for (auto &It : *LayerStack) {
            Globals::GlobalsContext::EventHandler->Subscribe(It, &Viper::Layers::Layer::OnEvent);
        }
    }

    void Window::UpdateWindowEvents() {
        glfwSetFramebufferSizeCallback(Context, [](GLFWwindow *Window, int Width, int Height) {
            Globals::GlobalsContext::EventHandler->Commit(new WindowFrameBufferSizeEvent(Width, Height));
        });

        glfwSetWindowSizeCallback(Context, [](GLFWwindow *Window, int Width, int Height) {
            Globals::GlobalsContext::EventHandler->Commit(new WindowResizeEvent(Width, Height));
        });

        glfwSetWindowPosCallback(Context, [](GLFWwindow *Window, int X, int Y) {
            Globals::GlobalsContext::EventHandler->Commit(new WindowPositionEvent(X, Y));
        });

        glfwSetWindowContentScaleCallback(Context, [](GLFWwindow *Window, float XScale, float YScale) {
            Globals::GlobalsContext::EventHandler->Commit(new WindowContentScaleEvent(XScale, YScale));
        });

        glfwSetWindowMaximizeCallback(Context, [](GLFWwindow *Window, int Maximized) {
            Globals::GlobalsContext::EventHandler->Commit(new WindowMaximizationEvent(Maximized));
        });

        glfwSetWindowFocusCallback(Context, [](GLFWwindow *Window, int Focused) {
            Globals::GlobalsContext::EventHandler->Commit(new WindowFocusEvent(Focused));
        });

        glfwSetWindowCloseCallback(Context, [](GLFWwindow *Window) {
            Globals::GlobalsContext::EventHandler->Commit(new WindowCloseEvent());
        });
    }

    void Window::Update() const {
        glfwSwapBuffers(Context);
        glfwPollEvents();
        Input::Input::ResetScroll();
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

    void Window::OnWindowFrameBufferSizeEvent(WindowFrameBufferSizeEvent *E) {
        spdlog::info("WindowFrameBufferSize Event triggered! Updated viewport to: {0}x{1}", E->Width, E->Height);
        glViewport(0, 0, E->Width, E->Height);
        WindowParams.Width = E->Width;
        WindowParams.Height = E->Height;
        Globals::GlobalsContext::Renderer2D->ResizeFBO(E->Width, E->Height);
    }

    void Window::OnWindowResizeEvent(WindowResizeEvent *E) {
        spdlog::info("WindowResize Event triggered! New size is {0}x{1}", E->Width, E->Height);
        WindowParams.Width = E->Width;
        WindowParams.Height = E->Height;
        Globals::GlobalsContext::Renderer2D->ResizeFBO(E->Width, E->Height);
    }

    void Window::OnWindowPositionEvent(WindowPositionEvent *E) {
        spdlog::info("WindowPosition Event triggered! New position is X: {0} : Y: {1}", E->X, E->Y);
    }

    void Window::OnWindowContentScaleEvent(WindowContentScaleEvent *E) {
        spdlog::info("WindowContentScale Event triggered! Scale (X:Y): {0}:{1}", E->XScale, E->YScale);
    }

    void Window::OnWindowMaximizationEvent(WindowMaximizationEvent *E) {
        spdlog::info("WindowMaximization Event triggered! Value: {0}", E->Maximized);
    }

    void Window::OnWindowFocusEvent(WindowFocusEvent *E) {
        spdlog::info("WindowFocus Event triggered! Focus: {0}", E->Focused);
    }

    void Window::OnWindowCloseEvent(WindowCloseEvent *E) {
        spdlog::info("WindowCloseEvent Event triggered!");
    }
}