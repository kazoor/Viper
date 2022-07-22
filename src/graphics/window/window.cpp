// Copyright (c) 2022.

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include "window.hpp"
#include <glm/gtc/matrix_transform.hpp> // ortho
#include "../../graphics/shaders/shader/shader.hpp"
#include "../../util/input/inputhandler/inputhandler.hpp"
#include "../../graphics/renderer/renderer.hpp"

namespace Viper::Graphics {
    void Window::FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height) {
        glViewport(0, 0, Width, Height);

    }

    Window::Window(int Width, int Height, const std::string &WindowName) {
        glfwInit();

        WindowEvents = new Events::EventBus();

        WindowParams = {
                Width,
                Height,
                WindowName,
                nullptr,
                nullptr,
                this->WindowEvents};

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        Context = CreateWindowEx(WindowParams);
        AspectRatio = static_cast< float >( WindowParams.Width ) / static_cast< float >( WindowParams.Height );

        if (!Context) {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
        }

        glfwMakeContextCurrent(Context);
        glfwSetFramebufferSizeCallback(Context, FramebufferSizeCallback);
        glfwSwapInterval(1); // Vsync on for now.

        glfwSetWindowUserPointer(Context, &WindowParams);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            spdlog::error("Failed to initialize GLAD");
        }

        // WindowEvents subscriptions.
        WindowEvents->Subscribe(this, &Window::OnWindowResizeEvent);
        WindowEvents->Subscribe(this, &Window::OnWindowPositionEvent);
        WindowEvents->Subscribe(this, &Window::OnWindowCloseEvent);

        glfwSetWindowSizeCallback(Context, [](GLFWwindow *Window, int Width, int Height) {
            WindowParams_t& WindowData = *(WindowParams_t*)glfwGetWindowUserPointer(Window);
            WindowData.EventCallback->Commit(new WindowResizeEvent(Width, Height));
        });

        glfwSetWindowPosCallback(Context, [](GLFWwindow *Window, int X, int Y) {
            WindowParams_t& WindowData = *(WindowParams_t*)glfwGetWindowUserPointer(Window);
            WindowData.EventCallback->Commit(new WindowPositionEvent(X, Y));
        });

        glfwSetWindowCloseCallback(Context, [](GLFWwindow *Window) {
            WindowParams_t& WindowData = *(WindowParams_t*)glfwGetWindowUserPointer(Window);
            WindowData.EventCallback->Commit(new WindowCloseEvent());
        });

        Shader Shader("resources/test.vert", "resources/test.frag");

        Renderer::Renderer2D* g_pRenderer = new Renderer::Renderer2D();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui_ImplGlfw_InitForOpenGL(Context, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        while (!glfwWindowShouldClose(Context)) {
            ProcessInput(Context);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            g_pRenderer->Begin();
            
            for( int y = -5; y < 20; y++ )
                for( int x = -5; x < 20; x++ )
                    g_pRenderer->DrawQuad(glm::vec2(x, y), (x + y) % 2 ? RendererAPI::Color(0.1f, 0.1f, 0.1f, 1.0f) : RendererAPI::Color(1.0f, 1.0f, 1.0f, 1.0f));

            g_pRenderer->Flush();
            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();
            
            
            if( ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
                ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
                ImGui::SetWindowSize(ImVec2(static_cast< float >( WindowParams.Width ), static_cast< float >( WindowParams.Height ) ));
                static auto m_dock_space = ImGui::GetID("m_view_id");
                ImGui::DockSpace(m_dock_space, ImVec2(0,0));
                ImGui::End();
            }
            ImGui::ShowDemoWindow();
            
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            auto m_ViewMatrix = glm::mat4(1.0f);

            auto m_ProjectionMatrix = glm::ortho(-AspectRatio * 30.0f, AspectRatio * 30.0f, 30.0f, -30.0f, 1.0f, -1.0f);
            auto m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

            Shader.Use();
            Shader.SetUniformMat4("u_Transform", m_Transform);
            Shader.SetUniformMat4("u_ViewProjection", m_ViewProjectionMatrix);

            g_pRenderer->End();
            
            Update();
        }
        delete g_pRenderer;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(Context);
        glfwTerminate();
    }

    GLFWwindow *Window::CreateWindowEx(WindowParams_t Params) {
        return glfwCreateWindow(Params.Width, Params.Height, Params.Title.c_str(), Params.Monitor, Params.Share);
    }

    void Window::ProcessInput(GLFWwindow *Window) {
        // if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // glfwSetWindowShouldClose(Window, true);
    }

    void Window::Update() const {
        glfwSwapBuffers(Context);
        glfwPollEvents();
        Input::Input::ResetScroll();
    }

    bool Window::Closed() const {
        return glfwWindowShouldClose(Context);
    }

    void Window::OnWindowResizeEvent(WindowResizeEvent *E) {
        spdlog::info("Window size change detected! New size is {0}x{1}", E->Width, E->Height);

        AspectRatio = static_cast< float >( E->Width ) / static_cast< float >( E->Height );

        WindowParams.Width = E->Width;
        WindowParams.Height = E->Height;
    }

    void Window::OnWindowPositionEvent(WindowPositionEvent *E) {
        spdlog::info("Window Position change detected! New position is X: {0} : Y: {1}", E->X, E->Y);
    }

    void Window::OnWindowCloseEvent(WindowCloseEvent *E) {
        spdlog::info("WindowCloseEvent() Called!");
        delete WindowEvents;
    }
}