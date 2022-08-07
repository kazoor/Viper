#include "viewport.hpp"
#include <glad/glad.h>
#include <graphics/window/window.hpp>


#include <imguieditor/fontawesome5.hpp>

#include <util/globals/global.hpp>

#include <ImGui/imgui_internal.h>
#include <ImGui/ImGuizmo.h>

#include <graphics/renderer/rendercommand.hpp>

#include <scene/entitycomponents.hpp>
#include <scene/sceneentity.hpp>

#include <viper/base.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace Viper {

    SceneViewport::SceneViewport( Scene* SceneContext, void* WindowContext ) : m_Context( SceneContext ), m_WindowContext( WindowContext ) { 
	    
    };

    // FBO Texture, should be rendered here.
    void SceneViewport::OnImGuiRender( Timestep::Timestep ts ) {
        Graphics::WindowParams_t& windowdata = *( Graphics::WindowParams_t* )glfwGetWindowUserPointer( ( GLFWwindow* )m_WindowContext );
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin(VIPER_TITLE, NULL,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
            ImGui::SetWindowPos(ImVec2(0.0f, 20.0f));
            ImGui::SetWindowSize(
                    ImVec2(static_cast< float >( windowdata.Width ), static_cast< float >( windowdata.Height ) - 20.0f ));

            ImGuiID DockspaceID = ImGui::GetID("Dockspace");
            ImGui::DockSpace(DockspaceID, ImVec2(0,0),ImGuiDockNodeFlags_None);

            ImGui::End();
        }
        ImGui::PopStyleVar();

        OnImGuiScene( ts );
        OnImGuiPlay( ts );
        OnImGuiShader( ts );
    };

    void SceneViewport::OnImGuiScene( Timestep::Timestep ts ) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin(ICON_FA_GAMEPAD "  Scene")) {
            ViewportSize = ImGui::GetContentRegionAvail();
            ImVec2 ScenePos = ImGui::GetWindowPos();
            Globals::Editor::SceneW = ViewportSize.x;
            Globals::Editor::SceneH = ViewportSize.y;

            if(RenderCommand::GetFramebufferWidth( ) != ( int )ViewportSize.x || RenderCommand::GetFramebufferHeight( ) != ( int )ViewportSize.y) {
                RenderCommand::Resize(( int )ViewportSize.x, ( int )ViewportSize.y);
                m_Context->OnViewportResize(( int )ViewportSize.x, ( int )ViewportSize.y);
            };

            Globals::Editor::IsSceneHovered = ImGui::IsWindowHovered();
            Globals::Editor::IsSceneFocused = ImGui::IsWindowFocused();

             ImGui::Image( reinterpret_cast< ImTextureID >( RenderCommand::GetColorAttachment( ) ),
                    ImVec2(ViewportSize.x, ViewportSize.y),
                    ImVec2( 0, 1 ), ImVec2( 1, 0 ) );


            auto selected_entity = m_Context->GetSelectedEntity();
            if( selected_entity ) {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                float windowWidth = ( float )ImGui::GetWindowWidth();
                float windowHeight = ( float )ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                auto cameraEntity = m_Context->GetCameraEntity();
                if( cameraEntity && cameraEntity != selected_entity ) {
                    const auto& camera = cameraEntity.get< CameraComponent >().camera;
                    const glm::mat4& cameraProjection = camera.GetProjection();
                    glm::mat4 cameraView = glm::inverse(cameraEntity.get< TransformComponent >().GetTransform());

                    auto& tc = selected_entity.get< TransformComponent >();
                    glm::mat4 transform = tc.GetTransform();

                    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                            ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transform));

                    if(ImGuizmo::IsUsing())
                        tc.Translation = glm::vec3(transform[3]);
                }
            };

            //if (ImGui::IsItemClicked())
            //    m_Context->ResetViewport();

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

    void SceneViewport::OnImGuiShader(Timestep::Timestep ts) {
        
    };
};