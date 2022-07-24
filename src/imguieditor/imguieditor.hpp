#pragma once

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include "../layers/layer/layer.hpp"
#include "../events/event/event.hpp"
#include "../graphics/window/window.hpp"
#include "../util/globals/global.hpp"

namespace Viper {
    struct OnLayerUpdateEvent : public Viper::Events::Event {
        OnLayerUpdateEvent() {}
    };

    class ImGuiEditor : public Layers::Layer {
    public:
        ImGuiEditor(Viper::Graphics::Window *Window) : Layer("ImGui Editor"), WindowContext(Window) {
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            ImGuiStyle& style = ImGui::GetStyle();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui_ImplGlfw_InitForOpenGL(WindowContext->Ctx(), true);
            ImGui_ImplOpenGL3_Init("#version 130");
            ImGui::StyleColorsDark();

            style.Colors[ ImGuiCol_Button ] = ImColor(35, 35, 35, 255 );
            style.Colors[ ImGuiCol_ButtonHovered ] = ImColor(40, 40, 40, 255 );
            style.Colors[ ImGuiCol_ButtonActive ] = ImColor(45, 45, 45, 255 );


            style.Colors[ ImGuiCol_FrameBg ] = ImColor(35, 35, 35, 255 );
            style.Colors[ ImGuiCol_FrameBgHovered ] = ImColor(40, 40, 40, 255 );
            style.Colors[ ImGuiCol_FrameBgActive ] = ImColor(45, 45, 45, 255 );

            style.Colors[ ImGuiCol_TitleBg ] = ImColor(35, 35, 35, 255 );
            style.Colors[ ImGuiCol_TitleBgActive ] = ImColor(40, 40, 40, 255 );
            style.Colors[ ImGuiCol_TitleBgCollapsed ] = ImColor(45, 45, 45, 255 );

            style.Colors[ ImGuiCol_Tab ] = ImColor(35, 35, 35, 255 );
            style.Colors[ ImGuiCol_TabHovered ] = ImColor(40, 40, 40, 255 );
            style.Colors[ ImGuiCol_TabActive ] = ImColor(45, 45, 45, 255 );
            style.Colors[ ImGuiCol_TabUnfocused ] = ImColor(70, 70, 70, 255 );
            style.Colors[ ImGuiCol_TabUnfocusedActive ] = ImColor(80, 80, 80, 255 );

            style.Colors[ ImGuiCol_Header ] = ImColor(35, 35, 35, 255 );
            style.Colors[ ImGuiCol_HeaderHovered ] = ImColor(40, 40, 40, 255 );
            style.Colors[ ImGuiCol_HeaderActive ] = ImColor(45, 45, 45, 255 );

            style.Colors[ ImGuiCol_SliderGrab ] = ImColor(50, 50, 50, 255 );
            style.Colors[ ImGuiCol_SliderGrabActive ] = ImColor(50, 50, 50, 255 );

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
            if(ImGui::Begin("Scene")) {
                ImVec2 SceneSize = ImGui::GetContentRegionAvail();
                ImGui::Image(reinterpret_cast< ImTextureID* >( Globals::Renderer2D->GetTexturebufferID() ), ImVec2(SceneSize.x,SceneSize.y));
                ImGui::End();
            };
            ImGui::PopStyleVar();

            if(ImGui::Begin("Hierarchy")) {
                ImGui::DragFloat("Zoom Level", &Globals::Editor::ZoomLevel, 1.0f, -200.0f, 200.0f);
                ImGui::DragFloat2("Position", Globals::Editor::Position, 1.0f, -100.0f, 100.0f);
                ImGui::DragFloat("Radians", &Globals::Editor::Radians, 1.0f, -180.0f, 180.0f);
                ImGui::DragFloat("Light Density", &Globals::Editor::LightDensity, 1.0f, -180.0f, 180.0f);

                ImGui::Separator();
                if( ImGui::Button("New GameObject")) {
                    auto go = std::make_unique< Viper::Components::GameObject >( );
                    go->AddComponent< Viper::Components::Transform >( glm::vec3(1.0f,2.0f,3.0f) );
                    Globals::Gom->OnAdd( std::move( go ) );

                    Globals::Editor::m_Errors.push_back({ "GameObject", "A new gameobject has been added."});
                };
                
                ImGui::Text("GameObjects: %i", Globals::Gom->GameObjectSize());
                ImGui::End();
            };

            if(ImGui::Begin("Debug Console")) {
                for( auto info : Globals::Editor::m_Errors ) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), info.StringType.c_str( ) );
                    ImGui::Text(info.ConsoleMessage.c_str( ) );
                    ImGui::Separator();
                }
                ImGui::End();
            };

            if(ImGui::Begin("Inspector")) {
                for( auto& go : Globals::Gom->m_GameObjects ) {
                    go->OnEditor();
                };
                ImGui::End();
            };

            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void OnEvent(Viper::Events::Event *Event) override {
            Viper::Globals::EventHandler->Commit(new OnLayerUpdateEvent());
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