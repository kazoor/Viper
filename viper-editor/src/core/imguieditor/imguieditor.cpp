#include "imguieditor.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/ImGuizmo.h>

#include <components/camera.hpp>
#include <components/spriterenderer.hpp>
#include <components/scripting.hpp>
#include <components/rigidbody2d.hpp>
#include <components/boxcollision2d.hpp>

#include <ghc/filesystem.hpp>
#include <util/timer/timer.hpp>
#include <viper/base.hpp>

namespace Viper {
    template< typename T >
    void ImGui_CreateComponent( const char* button_name, Ref< Components::GameObject >& object ) {
        if(!object->HasComponent< T >( ) ) {
            if( ImGui::MenuItem( button_name ) ) {
                object->AddComponent< T >( object.get() );
            };
        }
    };

    struct Filemanager_t {
        std::string filename;
        std::string fileext;
    };

    ImGuiEditor::ImGuiEditor(void* window)  : Layer("ImGui Editor"), WindowContext((GLFWwindow*)window) {
            
    };

    ImGuiEditor::~ImGuiEditor() {
        Destroy();
    }

    void ImGuiEditor::OnUpdate(Timestep::Timestep ts) {
            Graphics::WindowParams_t &WindowData = *(Graphics::WindowParams_t *) glfwGetWindowUserPointer(
                    WindowContext);

            

            ImGui_OnViewport(WindowData);
//
            ImGui_OnScene();
//
            ImGui_OnHierarchy(ts);
//
            ImGui_OnInspector();
//
            ImGui_OnConsole();
//
            ImGUi_OnPlaymode();

            ImGui_OnFileExplorer();

            ImGui::ShowDemoWindow();
            
            ImGui::EndFrame();

            ImGui::Render();
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiEditor::OnEvent(Events::Event& event) {
        Events::EventDispatcher dispatch(event);
        dispatch.Dispatch< Events::MouseScrollEvent >( VIPER_GET_EVENT_FUNC(ImGuiEditor::MouseScrollEvent));
    };

    void ImGuiEditor::Destroy() {
        
    }

    bool ImGuiEditor::MouseScrollEvent(Events::MouseScrollEvent& E) {
        VIPER_LOG("MouseScrollEvent Event triggered! {0}, {1}", E.x, E.y);
        if( !Globals::Editor::isPlaying )
            Globals::Editor::ZoomLevel -= static_cast< float >( E.y );
        return true;
    };

    void ImGuiEditor::ImGui_OnInspector()
        {
            
        };

        void ImGuiEditor::ImGui_OnScene() {
            
        }

        void ImGuiEditor::ImGui_OnConsole() {
            
        };

        constexpr char* s_Directory = "resources";
        void ImGuiEditor::ImGui_OnFileExplorer() {

            
        };

        void ImGuiEditor::ImGui_OnHierarchy(Timestep::Timestep ts)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
            if (ImGui::Begin(ICON_FA_SITEMAP "  Hierarchy")) {
                ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f ), "=======This section will be deleted soon.=====");
                ImGui::DragFloat("Zoom Level", &Globals::Editor::ZoomLevel, 1.0f, -200.0f, 200.0f);
                ImGui::DragFloat2("Position", Globals::Editor::Position, 1.0f, -100.0f, 100.0f);
                ImGui::DragFloat("Radians", &Globals::Editor::Radians, 1.0f, -180.0f, 180.0f);
                ImGui::DragFloat("Light Density", &Globals::Editor::LightDensity, 0.1f, -10.0f, 10.0f);
                ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f ), "==============================================");
                ImGui::Separator();
                static char buff[80];
                ImGui::SetNextItemWidth(100.0f);
                ImGui::InputText("##GameObjectName", buff, 80);
                ImGui::PopItemWidth();
                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                if (ImGui::Button(ICON_FA_PLUS " Add GameObject")) {
                    if (strlen(buff) > 0) {
                        auto go = Components::GameObject::Create(buff);
                        [&](Ref< Components::GameObject >&) {
                        }(go);
                        
                        Globals::GlobalsContext::Gom->OnAdd( go );
                        Globals::ConsoleContext::AddLog( VIPER_ICON_INFO " GameObject Spawn.", VIPER_FORMAT_STRING("GameObject: %s has been spawned!", buff), Globals::ConsoleInfo);
                        buff[0] = '\0';
                        Globals::Editor::SelectedObject++;
                    };
                };
                ImGui::PopStyleColor();
                if( ImGui::Button(ICON_FA_PLUS " Add PlayerController")) {
                    //auto go = std::make_unique<Viper::Components::GameObject>("PlayerController");

                    auto go = Components::GameObject::Create("PlayerController");
                    [&](Ref< Components::GameObject >& c) {
                        c->AddComponent< Components::Camera >(c.get());
                        c->AddComponent< Components::SpriteRenderer >(c.get());
                        c->AddComponent< Components::Rigidbody2D >(c.get());
                    }(go);

                    Globals::GlobalsContext::Gom->OnAdd( go );
                    Globals::ConsoleContext::AddLog( VIPER_ICON_INFO " GameObject Spawn.", VIPER_FORMAT_STRING("GameObject: PlayerController has been spawned!", buff), Globals::ConsoleInfo);
                    Globals::Editor::SelectedObject++;
                };

                ImGui::Separator();

                std::size_t goSize = 0;
                for (auto &go: *Globals::GlobalsContext::Gom ) {
                    ImGuiTreeNodeFlags flag = ((Globals::Editor::SelectedObject == goSize) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
                    bool is_open = ImGui::TreeNodeEx(go->tag.c_str(), flag, go->tag.c_str());
                    if (ImGui::IsItemClicked())
                        Globals::Editor::SelectedObject = goSize;

                    if (is_open) {
                        if (ImGui::Button(std::string(ICON_FA_TRASH_ALT " Remove ").append(go->tag).c_str())) {
                            Globals::GlobalsContext::Gom->pop(goSize + 1);
                            Globals::Editor::SelectedObject = -1;
                        }
                        ImGui::TreePop();
                    }
                    goSize++;
                };

                ImGui::Text("GameObjects: %i", Globals::GlobalsContext::Gom->GameObjectSize());
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", ts.milliseconds(),
                           1.0f / ts.seconds() );
                ImGui::Text("Quads Rendered: %i", Renderer::Renderer2D::GetQuadCount());
                ImGui::Text("Indices used: %i", Renderer::Renderer2D::GetIndexCount());
                ImGui::Text("Lines used: %i", Renderer::Renderer2D::GetLineCount());

                ImGui::End();
            };
            ImGui::PopStyleVar();
        };

        void ImGuiEditor::ImGui_OnViewport( const Graphics::WindowParams_t& WindowData)
        {
            
        };

        void ImGuiEditor::ImGUi_OnPlaymode() {
            
        };
};