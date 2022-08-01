#include "inspector.hpp"

#include <imguieditor/fontawesome5.hpp>
#include <ImGui/imgui.h>

namespace Viper::Editor {
    SceneInspector::SceneInspector( const Ref< Scene >& SceneContext ) : m_Context( SceneContext ) { };

    void SceneInspector::OnImGuiRender( Timestep::Timestep ts ) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
        if (ImGui::Begin(ICON_FA_CUBE "  Inspector")) {
            //if (Globals::Editor::SelectedObject != -1) {
            //    auto go = Globals::GlobalsContext::Gom->get(Globals::Editor::SelectedObject);
            //    go->OnEditor();
            //    go->OnDeletion();
//
            //    //if(ImGui::Button( ICON_FA_PLUS " Add Component" ) && ImGui::IsMouseClicked(1) )
            //    //{
            //    //    bool is_widgets_open = ImGui::BeginPopup("##combo_widgets" );
            //    //    if( is_widgets_open )
            //    //        ImGui::Text("hello");
            //    //    ImGui::EndPopup();
            //    //};
//
            //    ImGui::Button( ICON_FA_PLUS " Add Component" );
            //    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            //    if (ImGui::BeginPopupContextWindow(0, 1, false))
            //    {
            //        ImGui_CreateComponent< Components::Camera >( "Camera", go );
            //        ImGui_CreateComponent< Components::SpriteRenderer >( "Sprite Renderer", go );
            //        ImGui_CreateComponent< Components::TestScript >( "Test Script", go );
            //        ImGui_CreateComponent< Components::Rigidbody2D >( "Rigidbody2D", go );
            //        ImGui_CreateComponent< Components::BoxCollider2D >( "BoxCollider2D", go );
//
            //        ImGui::EndPopup();
            //    }
            //    ImGui::PopStyleVar();
            //    ImGui::Separator();
            //}
            ImGui::End();
        };
        ImGui::PopStyleVar();
    };
};