#pragma once

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include "../layers/layer/layer.hpp"
#include "../events/event/event.hpp"
#include "../graphics/window/window.hpp"
#include "../util/globals/global.hpp"
#include "../components/boxcollision2d.hpp"
#include "../util/input/input.hpp"
#include "../util/input/keycodes.hpp"

namespace Viper {
    struct OnLayerUpdateEvent : public Viper::Events::Event {
        OnLayerUpdateEvent() {}
    };

    class ImGuiEditor : public Layers::Layer {
    public:
        ImGuiEditor(Viper::Graphics::Window *Window) : Layer("ImGui Editor"), WindowContext(Window) {
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            ImGuiStyle &style = ImGui::GetStyle();
            ImGui::StyleColorsDark();

            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/assets/fonts/JetBrainsMonoNL-Bold.ttf", 14.0f);

            style.Colors[ImGuiCol_Button] = ImColor(35, 35, 35, 255);
            style.Colors[ImGuiCol_ButtonHovered] = ImColor(40, 40, 40, 255);
            style.Colors[ImGuiCol_ButtonActive] = ImColor(45, 45, 45, 255);

            style.Colors[ImGuiCol_FrameBg] = ImColor(35, 35, 35, 255);
            style.Colors[ImGuiCol_FrameBgHovered] = ImColor(40, 40, 40, 255);
            style.Colors[ImGuiCol_FrameBgActive] = ImColor(45, 45, 45, 255);

            style.Colors[ImGuiCol_TitleBg] = ImColor(35, 35, 35, 255);
            style.Colors[ImGuiCol_TitleBgActive] = ImColor(40, 40, 40, 255);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(45, 45, 45, 255);

            style.Colors[ImGuiCol_Tab] = ImColor(35, 35, 35, 255);
            style.Colors[ImGuiCol_TabHovered] = ImColor(40, 40, 40, 255);
            style.Colors[ImGuiCol_TabActive] = ImColor(45, 45, 45, 255);
            style.Colors[ImGuiCol_TabUnfocused] = ImColor(70, 70, 70, 255);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(80, 80, 80, 255);

            style.Colors[ImGuiCol_Header] = ImColor(35, 35, 35, 255);
            style.Colors[ImGuiCol_HeaderHovered] = ImColor(40, 40, 40, 255);
            style.Colors[ImGuiCol_HeaderActive] = ImColor(45, 45, 45, 255);

            style.Colors[ImGuiCol_SliderGrab] = ImColor(50, 50, 50, 255);
            style.Colors[ImGuiCol_SliderGrabActive] = ImColor(50, 50, 50, 255);

            ImGui_ImplGlfw_InitForOpenGL(WindowContext->Ctx(), true);
            ImGui_ImplOpenGL3_Init("#version 410");

            style.GrabRounding = 2.0f;

            WindowPaddingReserved = style.WindowPadding;
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

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            if (ImGui::Begin("Scene")) {
                ImVec2 SceneSize = ImGui::GetContentRegionAvail();
                ImGui::Image(
                        reinterpret_cast< ImTextureID * >( Globals::GlobalsContext::Renderer2D->GetTexturebufferID()),
                        ImVec2(SceneSize.x, SceneSize.y));
                if (ImGui::IsItemClicked())
                    Globals::Editor::SelectedObject = -1;
                ImGui::End();
            };
            ImGui::PopStyleVar();

            if (ImGui::Begin("Hierarchy")) {
                ImGui::DragFloat("Zoom Level", &Globals::Editor::ZoomLevel, 1.0f, -200.0f, 200.0f);
                ImGui::DragFloat2("Position", Globals::Editor::Position, 1.0f, -100.0f, 100.0f);
                ImGui::DragFloat("Radians", &Globals::Editor::Radians, 1.0f, -180.0f, 180.0f);
                ImGui::DragFloat("Light Density", &Globals::Editor::LightDensity, 0.1f, -10.0f, 10.0f);

                ImGui::Separator();
                static char buff[80];
                ImGui::SetNextItemWidth(100.0f);
                ImGui::InputText("##GameObjectName", buff, 80);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Add GameObject")) {
                    if (strlen(buff) > 1) {
                        auto go = std::make_unique<Viper::Components::GameObject>(buff);
                        go->AddComponent<Viper::Components::Transform>(glm::vec3(0.0f, 0.0f, 0.0f),
                                                                       glm::vec3(1.0f, 1.0f, 0.0f),
                                                                       glm::vec3(0.0f, 0.0f, 0.0f));

                        Globals::GlobalsContext::Gom->OnAdd(std::move(go));

                        Globals::ConsoleContext::AddLog("GameObject", "A new gameobject has been added.");
                        buff[0] = '\0';
                        Globals::Editor::SelectedObject++;
                    };
                };

                ImGui::Separator();

                std::size_t goSize = 0;
                for (auto &go: Globals::GlobalsContext::Gom->m_GameObjects) {
                    ImGuiTreeNodeFlags flag =
                            ((Globals::Editor::SelectedObject == goSize) ? ImGuiTreeNodeFlags_Selected : 0) |
                            ImGuiTreeNodeFlags_OpenOnArrow;
                    bool is_open = ImGui::TreeNodeEx(go->tag.c_str(), flag, go->tag.c_str());
                    if (ImGui::IsItemClicked())
                        Globals::Editor::SelectedObject = goSize;

                    if (is_open) {
                        if (ImGui::Button(std::string("Remove ").append(go->tag).c_str())) {
                            Globals::GlobalsContext::Gom->m_GameObjects.erase(
                                    Globals::GlobalsContext::Gom->m_GameObjects.begin() + (goSize + 1));
                            Globals::Editor::SelectedObject = -1;
                        }
                        ImGui::TreePop();
                    }
                    goSize++;
                };

                ImGui::Text("GameObjects: %i", Globals::GlobalsContext::Gom->GameObjectSize());
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1.0f / Globals::Editor::DeltaTime,
                            1000.0f * Globals::Editor::DeltaTime);
                ImGui::End();
            };

            if (ImGui::Begin("Debug Console")) {
                for (auto info: Globals::ConsoleContext::GetLogs()) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), info.StringType.c_str());
                    ImGui::Text(info.ConsoleMessage.c_str());
                    ImGui::Separator();
                }
                ImGui::End();
            };

            if (ImGui::Begin("Inspector")) {
                if (Globals::Editor::SelectedObject != -1) {
                    auto &go = Globals::GlobalsContext::Gom->m_GameObjects.at(Globals::Editor::SelectedObject);

                    go->OnEditor();
                    if (go->HasComponent<Components::Transform>()) {
                        ImGui::Separator();
                        if (ImGui::Button("Remove Transform"))
                            go->RemoveComponent<Components::Transform>();
                        ImGui::Separator();
                    }

                    ImGuiTreeNodeFlags t =
                            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
                    if (ImGui::TreeNodeEx("BoxCollision2D", t)) {
                        if (!go->HasComponent<Components::BoxCollision2D>() && ImGui::Button("Add BoxCollision2D")) {
                            auto &go = Globals::GlobalsContext::Gom->m_GameObjects.at(Globals::Editor::SelectedObject);
                            go->AddComponent<Components::BoxCollision2D>(go.get());
                        } else if (go->HasComponent<Components::BoxCollision2D>() && ImGui::Button("Remove")) {
                            go->RemoveComponent<Components::BoxCollision2D>();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::Separator();

                }

                ImGui::End();
            };

            ImGui::ShowDemoWindow();

            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void OnEvent(Viper::Events::Event *Event) override {
            Globals::GlobalsContext::EventHandler->Commit(new OnLayerUpdateEvent());
        }

        void Destroy() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

    private:
        Viper::Graphics::Window *WindowContext;
        ImVec2 WindowPaddingReserved;
    };
}