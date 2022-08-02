#include "inspector.hpp"

#include <imguieditor/fontawesome5.hpp>
#include <ImGui/imgui.h>

#include <scene/entitycomponents.hpp>
#include <scene/sceneentity.hpp>
#include <scene/scene.hpp>

static void ImGuiTransform3F(const std::string& string, glm::vec3& values, float reset_value = 0.0f) {
    ImGui::Columns(2, std::string("##").append(string).c_str( ), false);
    ImGui::SetColumnWidth(0, 70.0f);
    ImGui::Text( string.c_str( ) );
    ImGui::NextColumn();
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(230,53,97));
    if( ImGui::Button("X"))
        values.x = reset_value;
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string( "##" ).append( string ).append( "##X" ).c_str( ), &values.x, 0.1f);

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(37,206,113));
    if( ImGui::Button("Y"))
        values.y = reset_value;
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string( "##" ).append( string ).append( "##Y" ).c_str( ), &values.y, 0.1f);

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(41,105,169));
    if( ImGui::Button("Z"))
        values.z = reset_value;
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string( "##" ).append( string ).append( "##Z" ).c_str( ), &values.z, 0.1f);
    ImGui::PopStyleVar(1);
    ImGui::Columns(1);
};

namespace Viper {
    SceneInspector::SceneInspector( Scene* SceneContext ) : m_Context( SceneContext ) { };

    void SceneInspector::OnImGuiRender( Timestep::Timestep ts ) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
        if (ImGui::Begin(ICON_FA_CUBE "  Inspector")) {
            if( m_Context->m_selected_entity != entt::null ) {
                Entity ent = { m_Context->m_selected_entity, m_Context };

                if( ent.has< tag_t >( ) )
                    ImGui::Text("Component name: %s", ent.get< tag_t >( ).tag.c_str( ) );

                if( ent.has< transform_t >( ) )
                    ImGuiTransform3F("position", ent.get< transform_t >( ).pos, 0.0 );
                    //ImGui::Text("has transform.\n");
            };
            ImGui::End();
        };
        ImGui::PopStyleVar();
    };
};