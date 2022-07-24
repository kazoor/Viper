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

    static void ImGuiTransform3F(const std::string& string, glm::vec3& values, float reset_value = 0.0f) {
            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, 100.0f);
            ImGui::Text(string.c_str());
            ImGui::NextColumn();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            if(ImGui::Button("X"))
                values.x = reset_value;
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::SetNextItemWidth(40.0f);
            ImGui::DragFloat(std::string( "##" ).append( string ).append( "##X" ).c_str( ), &values.x, 0.1f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            if(ImGui::Button("Y"))
                values.y = reset_value;
            ImGui::PopStyleColor();

            ImGui::SameLine();
            ImGui::SetNextItemWidth(40.0f);
            ImGui::DragFloat(std::string( "##" ).append( string ).append( "##Y" ).c_str( ), &values.y, 0.1f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
            if(ImGui::Button("Z"))
                values.z = reset_value;
            ImGui::PopStyleColor();

            ImGui::SameLine();
            ImGui::SetNextItemWidth(40.0f);
            ImGui::DragFloat(std::string( "##" ).append( string ).append( "##Z" ).c_str( ), &values.z, 0.1f);
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();
            ImGui::Columns(1);
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
                    auto& tr = go->GetComponent< Components::Transform >( );
                    //ImGui::Text("%.2f %.2f %.2f", tr.position.x, tr.position.y, tr.position.z);
                    if( ImGui::TreeNode( "Transform Component" ) ) {

                        ImGuiTransform3F("Position", tr.position );
                        ImGuiTransform3F("Rotation", tr.rotation );
                        ImGuiTransform3F("Scale", tr.scale );
                        ImGui::TreePop();
                    };
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