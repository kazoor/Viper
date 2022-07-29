#include <string>
#include "transform.hpp"
#include <ImGui/imgui.h>
#include "../imguieditor/fontawesome5.hpp"

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

namespace Viper::Components {
    Transform::Transform() {
        position = glm::vec3(0.0f);
        scale = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);

        right = glm::vec3( 1.0f, 0.0f, 0.0f );
        up = glm::vec3( 0.0f, 1.0f, 0.0f );
        forward = glm::vec3( 0.0f, 0.0f, 1.0f );
        
        enabled = true;
    };

    Transform::Transform( glm::vec3 pos ) {
        position = pos;
        scale = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);

        right = glm::vec3( 1.0f, 0.0f, 0.0f );
        up = glm::vec3( 0.0f, 1.0f, 0.0f );
        forward = glm::vec3( 0.0f, 0.0f, 1.0f );

        enabled = true;
    };

    Transform::Transform( glm::vec3 pos, glm::vec3 sc ) {
        position = pos;
        scale = sc;
        rotation = glm::vec3(0.0f);

        right = glm::vec3( 1.0f, 0.0f, 0.0f );
        up = glm::vec3( 0.0f, 1.0f, 0.0f );
        forward = glm::vec3( 0.0f, 0.0f, 1.0f );

        enabled = true;
    };

    Transform::Transform( glm::vec3 pos, glm::vec3 sc, glm::vec3 rot ) {
        position = pos;
        scale = sc;
        rotation = rot;

        right = glm::vec3( 1.0f, 0.0f, 0.0f );
        up = glm::vec3( 0.0f, 1.0f, 0.0f );
        forward = glm::vec3( 0.0f, 0.0f, 1.0f );

        enabled = true;
    };

    void Transform::Editor() {
        ImGuiTreeNodeFlags t = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
        
        if(ImGui::TreeNodeEx( " " ICON_FA_CUBES "  Transform", t)) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
            ImGui::Checkbox("Enabled##Transform", &enabled);
            ImGui::PopStyleVar();

            if(!enabled)
                ImGui::BeginDisabled();

            ImGuiTransform3F("Position", position);
            ImGuiTransform3F("Rotation", rotation);
            ImGuiTransform3F("Scale", scale, 1.0f);
            
            if(!enabled)
                ImGui::EndDisabled();
            ImGui::TreePop();
        };
    };
};