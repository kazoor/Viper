#pragma once

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include "../layers/layer/layer.hpp"
#include "../events/event/event.hpp"
#include "../graphics/window/window.hpp"

namespace Viper {
    struct OnLayerUpdateEvent : public Viper::Events::Event {
        OnLayerUpdateEvent() {}
    };

    class ImGuiEditor : public Layers::Layer {
    public:
        ImGuiEditor(Viper::Graphics::Window *Window) : Layer("ImGui Editor"), WindowContext(Window) {
            ImGui::CreateContext();
            ImGuiIO &Io = ImGui::GetIO();
            Io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui_ImplGlfw_InitForOpenGL(WindowContext->Ctx(), true);
            ImGui_ImplOpenGL3_Init("#version 130");
        }

        ~ImGuiEditor() {
            Destroy();
        }

        void OnUpdate() override {
            Viper::Graphics::WindowParams_t &WindowData = *(Viper::Graphics::WindowParams_t *) glfwGetWindowUserPointer(
                    WindowContext->Ctx());

            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            if (ImGui::Begin("Viewport", NULL,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
                ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
                ImGui::SetWindowSize(
                        ImVec2(static_cast< float >( WindowData.Width ), static_cast< float >( WindowData.Height )));
                static auto m_dock_space = ImGui::GetID("m_view_id");
                ImGui::DockSpace(m_dock_space, ImVec2(0, 0));
                ImGui::End();
            }
            ImGui::ShowDemoWindow();

            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void OnEvent(Viper::Events::Event *Event) override {
            LayerEventHandler->Commit(new OnLayerUpdateEvent());
        }

        void Destroy() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

    private:
        Viper::Graphics::Window *WindowContext;
    };
}