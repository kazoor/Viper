#pragma once

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include "../layers/layer/layer.hpp"
#include "../events/event/event.hpp"
#include "../graphics/window/window.hpp"
#include "../components/camera.hpp"
#include "../components/spriterenderer.hpp"
#include "../components/scripting.hpp"
//#include "../components/boxcollision2d.hpp"
#include "../util/globals/global.hpp"
#include "../util/input/input.hpp"
#include "../util/input/keycodes.hpp"
#include "fontawesome5.hpp"
#include "../viper/base.hpp"

namespace Viper {
    
    class ImGuiEditor : public Layers::Layer {
    public:
        ImGuiEditor(Viper::Graphics::Window *Window) : Layer("ImGui Editor"), WindowContext(Window) {
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            ImGuiStyle &style = ImGui::GetStyle();
            ImGui::StyleColorsDark();

            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.Fonts->AddFontFromFileTTF("resources/assets/fonts/OpenSans-Bold.ttf", 16.0f);
            //io.Fonts->AddFontDefault();

            static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
            ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
            io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAS, 12.0f, &icons_config, icons_ranges );

            style.Colors[ImGuiCol_Text] = ImColor( 200, 200, 200, 255 );
            style.Colors[ImGuiCol_Button] = ImColor(35, 35, 35, 255);
            style.Colors[ImGuiCol_ButtonHovered] = ImColor(40, 40, 40, 255);
            style.Colors[ImGuiCol_ButtonActive] = ImColor(45, 45, 45, 255);

            style.Colors[ImGuiCol_FrameBg] = ImColor(14, 16, 39, 255);
            style.Colors[ImGuiCol_FrameBgHovered] = ImColor(17, 19, 47, 255);
            style.Colors[ImGuiCol_FrameBgActive] = ImColor(22, 25, 52, 255);

            style.Colors[ImGuiCol_Tab] = ImColor(17, 11, 44, 255);
            style.Colors[ImGuiCol_TabHovered] = ImColor(22, 17, 68, 255);
            style.Colors[ImGuiCol_TabActive] = ImColor(22, 17, 68, 255);
            style.Colors[ImGuiCol_TabUnfocused] = ImColor(17, 11, 44, 255);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(41, 29, 93, 255);

            style.Colors[ImGuiCol_Header] = ImColor(26, 16, 61, 255);
            style.Colors[ImGuiCol_HeaderHovered] = ImColor(37, 28, 68, 255);
            style.Colors[ImGuiCol_HeaderActive] = ImColor(36, 24, 83, 255);

            style.Colors[ImGuiCol_SliderGrab] = ImColor(50, 50, 50, 255);
            style.Colors[ImGuiCol_SliderGrabActive] = ImColor(50, 50, 50, 255);

            style.Colors[ImGuiCol_WindowBg] = ImColor(3, 3, 13, 255 );
            style.Colors[ImGuiCol_TitleBg] = ImColor(11, 9, 35, 255 );
            style.Colors[ImGuiCol_TitleBgActive] = ImColor(21, 18, 55, 255 );
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(21, 18, 55, 255 );

            style.Colors[ImGuiCol_Button] = ImColor(35, 27, 75, 255 );
            style.Colors[ImGuiCol_ButtonHovered] = ImColor(35, 27, 75, 255 );
            style.Colors[ImGuiCol_ButtonActive] = ImColor(34, 24, 81, 255 );

            style.Colors[ImGuiCol_SliderGrab] = ImColor( 50, 46, 117, 255 );
            style.Colors[ImGuiCol_SliderGrabActive] = ImColor( 65, 61, 139, 255 );

            style.Colors[ImGuiCol_ScrollbarGrab] = ImColor( 50, 46, 117, 255 );
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor( 65, 61, 139, 255 );
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor( 65, 61, 139, 255 );

            style.WindowBorderSize = 0;
            style.FrameBorderSize = 0;
            style.IndentSpacing = 0.0f;
            style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
            style.WindowPadding = ImVec2( 0, 8 );
            style.IndentSpacing = 10.0f;
            style.ScrollbarRounding = 2.0f;
            style.ScrollbarSize = 10.0f;

            ImGui_ImplGlfw_InitForOpenGL(WindowContext->Ctx(), true);
            ImGui_ImplOpenGL3_Init("#version 410");

            style.GrabRounding = 2.0f;
        }

        ~ImGuiEditor() {
            Destroy();
        }

        void OnUpdate() override {
            Graphics::Window::WindowParams_t &WindowData = *(Graphics::Window::WindowParams_t *) glfwGetWindowUserPointer(
                    WindowContext->Ctx());

            ImGui_ImplGlfw_NewFrame();

            ImGui_ImplOpenGL3_NewFrame();

            ImGui::NewFrame();

            ImGui_OnViewport(WindowData);

            ImGui_OnScene();

            ImGui_OnHierarchy();

            ImGui_OnInspector();

            ImGui_OnConsole();

            ImGUi_OnPlaymode();

            ImGui::EndFrame();

            ImGui::Render();
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void OnEvent(Events::Event& event) override {
            Events::EventDispatcher dispatch(event);
            dispatch.Dispatch< Events::MouseScrollEvent >( VIPER_GET_EVENT_FUNC(ImGuiEditor::MouseScrollEvent));
        }

        void Destroy() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        };

        bool MouseScrollEvent(Events::MouseScrollEvent& E) {
            VIPER_LOG("MouseScrollEvent Event triggered! {0}, {1}", E.x, E.y);
            if( !Globals::Editor::isPlaying )
                Globals::Editor::ZoomLevel -= static_cast< float >( E.y );
            return true;
        };
    private:
        void ImGui_OnInspector()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
            if (ImGui::Begin(ICON_FA_CUBE "  Inspector")) {
                if (Globals::Editor::SelectedObject != -1) {
                    auto go = Globals::GlobalsContext::Gom->get(Globals::Editor::SelectedObject);
                    go->OnEditor();
                    go->OnDeletion();
                    ImGui::Separator();
                }
                ImGui::End();
            };
            ImGui::PopStyleVar();
        };

        void ImGui_OnScene() {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            if (ImGui::Begin(ICON_FA_GAMEPAD "  Scene")) {
                ImVec2 SceneSize = ImGui::GetContentRegionAvail();
                ImGui::Image(
                        reinterpret_cast< ImTextureID * >( Renderer::Renderer2D::GetTexture()),
                        ImVec2(SceneSize.x, SceneSize.y));
                if (ImGui::IsItemClicked())
                    Globals::Editor::SelectedObject = -1;
                ImGui::End();
            };
            ImGui::PopStyleVar();
        }

        void ImGui_OnConsole() {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
            if(ImGui::Begin(ICON_FA_TERMINAL "  Debug Console")) {
                Globals::ConsoleContext::ResizeLogs( 10U );                    
                for( auto info : Globals::ConsoleContext::GetLogs( ) ) {
                    auto color = Globals::ConsoleContext::GetConsoleColor( info.Flag );
                    ImGui::TextColored(ImVec4(color[0], color[1], color[2], 1.0f), info.StringType.c_str( ) );
                    ImGui::Text(info.ConsoleMessage.c_str( ) );
                    ImGui::Separator();
                }
                ImGui::End();
            };
            ImGui::PopStyleVar();
        };

        void ImGui_OnHierarchy()
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
                    if (strlen(buff) > 1) {
                        //auto go = std::make_unique<Viper::Components::GameObject>(buff);
                        auto go = Components::GameObject::Create(buff);
                        [&](Components::GameObject* c) {
                            
                            //c->AddComponent< Viper::Components::SpriteRenderer >( c, glm::vec4(1.0f, 0.2f, 0.2f, 1.0f ) );
                            //c->AddComponent< Viper::Components::BoxCollision2D >( c );
                        }(go.get());

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
                    }(go);

                    Globals::GlobalsContext::Gom->OnAdd( go );
                    Globals::ConsoleContext::AddLog( VIPER_ICON_INFO " GameObject Spawn.", VIPER_FORMAT_STRING("GameObject: PlayerController has been spawned!", buff), Globals::ConsoleInfo);
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
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f * Globals::Editor::DeltaTime,
                           1.0f / Globals::Editor::DeltaTime );
                ImGui::Text("Quads Rendered: %i", Renderer::Renderer2D::GetQuadCount());
                ImGui::Text("Indices used: %i", Renderer::Renderer2D::GetIndexCount());

                ImGui::End();
            };
            ImGui::PopStyleVar();
        };

        void ImGui_OnViewport( const Graphics::Window::WindowParams_t& WindowData)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            if (ImGui::Begin(VIPER_TITLE, NULL,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
                ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
                ImGui::SetWindowSize(
                        ImVec2(static_cast< float >( WindowData.Width ), static_cast< float >( WindowData.Height )));
                static auto m_dock_space = ImGui::GetID("m_view_id");
                ImGui::DockSpace(m_dock_space, ImVec2(0, 0));
                ImGui::End();
            }
            ImGui::PopStyleVar();
        };

        void ImGUi_OnPlaymode() {
            if(ImGui::Begin("##PlayMode", NULL)) {
                float size = ImGui::GetWindowHeight( ) - 4.0f;
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - ( size * 0.5f ));
                if( ImGui::Button( !( Globals::Editor::isPlaying ) ? ICON_FA_PLAY : ICON_FA_SQUARE ) )
                    Globals::Editor::isPlaying = !Globals::Editor::isPlaying;
                ImGui::End();
            };
        };

    private:
        Viper::Graphics::Window *WindowContext;
    };
}