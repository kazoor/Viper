#include <glad/glad.h>
#include <graphics/window/window.hpp>
#include "viewport.hpp"
#include <imguieditor/fontawesome5.hpp>
#include <util/globals/global.hpp>
#include <viper/base.hpp>
#include <ImGui/imgui.h>
#include <graphics/renderer/rendercommand.hpp>

namespace Viper {
    SceneViewport::SceneViewport( Scene* SceneContext, void* WindowContext ) : m_Context( SceneContext ), m_WindowContext( WindowContext ) { };

    // FBO Texture, should be rendered here.
    void SceneViewport::OnImGuiRender( Timestep::Timestep ts ) {
        Graphics::WindowParams_t& windowdata = *( Graphics::WindowParams_t* )glfwGetWindowUserPointer( ( GLFWwindow* )m_WindowContext );
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin(VIPER_TITLE, NULL,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
            ImGui::SetWindowPos(ImVec2(0.0f, 20.0f));
            ImGui::SetWindowSize(
                    ImVec2(static_cast< float >( windowdata.Width ), static_cast< float >( windowdata.Height ) - 20.0f ));
            static auto m_dock_space = ImGui::GetID( "m_view_id" );
            ImGui::DockSpace(m_dock_space, ImVec2(0, 0));

            if(ImGui::BeginMenuBar()) {
                if(ImGui::MenuItem("sss"))
                    ImGui::Text("xxx");
                ImGui::EndMenuBar();
            };
            ImGui::End();
        }
        ImGui::PopStyleVar();

        OnImGuiScene( ts );
        OnImGuiPlay( ts );
    };

    void SceneViewport::OnImGuiScene( Timestep::Timestep ts ) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin(ICON_FA_GAMEPAD "  Scene")) {
            ImVec2 SceneSize = ImGui::GetContentRegionAvail();
            ImVec2 ScenePos = ImGui::GetWindowPos();
            Globals::Editor::SceneX = ScenePos.x;
            Globals::Editor::SceneY = ScenePos.y;

            Globals::Editor::SceneW = SceneSize.x;
            Globals::Editor::SceneH = SceneSize.y;

            ImGui::Image(
                    reinterpret_cast< ImTextureID >( RenderCommand::FrameID( ) ),
                    ImVec2(SceneSize.x, SceneSize.y),
                    ImVec2( 0, 1 ), ImVec2( 1, 0));

            if (ImGui::IsItemClicked())
                m_Context->ResetViewport();

            ImGui::End();
        };
        ImGui::PopStyleVar();
    };

    void SceneViewport::OnImGuiPlay( Timestep::Timestep ts ) {
        ImGuiWindowFlags playmode = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if(ImGui::Begin("##PlayMode", NULL, playmode)) {
            float size = ImGui::GetWindowHeight( ) - 4.0f;
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - ( size * 0.5f ));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0,0,0,0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0,0,0,0));
            if( ImGui::Button( !( Globals::Editor::isPlaying ) ? ICON_FA_PLAY : ICON_FA_SQUARE ) )
                Globals::Editor::isPlaying = !Globals::Editor::isPlaying;

            ImGui::PopStyleColor(0);
            ImGui::PopStyleColor(1);
            ImGui::PopStyleColor(2);

            ImGui::End();
        };
    };
};