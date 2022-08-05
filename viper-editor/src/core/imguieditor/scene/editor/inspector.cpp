#include "inspector.hpp"

#include <iostream>
#include <imguieditor/fontawesome5.hpp>
#include <ImGui/imgui.h>

#include <scene/entitycomponents.hpp>
#include <scene/sceneentity.hpp>
#include <scene/scene.hpp>
#include <components/transform.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include <util/jsonfilehandler/jsonfilehandler.hpp>

static void imgui_gizmo_transform(const std::string& string, glm::vec3& values, float reset_value = 0.0f) {
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
    static std::string m_Nullified = "";

    void SerializeEntity( Entity entity, std::uint32_t entity_id, nlohmann::json& data ) {
        TransformComponent tr;
        SpriteRendererComponent sr;

        if(entity.has<TransformComponent>()) {
            tr = entity.get<TransformComponent>();
            data.push_back({
                entity_id,
                {
                    {"Tag", entity.get< TagComponent >( ).tag.c_str( )},
                    {"Transform", {
                        {"Position", {
                            {"X", tr.position.x},
                            {"Y", tr.position.y},
                            {"Z", tr.position.z}
                        }},
                        {"Scale", {
                            {"X", tr.scale.x},
                            {"Y", tr.scale.y},
                            {"Z", tr.scale.z}
                        }},
                        {"Rotation", {
                            {"X", tr.rotation.x},
                            {"Y", tr.rotation.y},
                            {"Z", tr.rotation.z}
                        }}
                 }},
            }});
        }

        if(entity.has<SpriteRendererComponent>()) {
            sr = entity.get<SpriteRendererComponent>();
            data.push_back({
                entity_id,
                {
                    {"Tag", entity.get< TagComponent >( ).tag.c_str( )},
                    {"SpriteRenderer", {
                        {"Color", {
                            {"R", sr.color.r},
                            {"G", sr.color.g},
                            {"B", sr.color.b},
                            {"A", sr.color.a}
                        }},
                        {"Sprite", {
                            {"Width", sr.sprite.get()->GetWidth()},
                            {"Height", sr.sprite.get()->GetHeight()},
                            {"Path", sr.sprite.get()->GetPath()},
                            {"SpriteID", sr.sprite.get()->GetSpriteID()},
                            {"Tiling", sr.tiling}
                        }}
                    }}
                }
            });
        }
    }

    void SceneInspector::OnImGuiPopulateComponents( Entity entity ) {
        if( ImGui::MenuItem("SpriteRenderer") && !entity.has< SpriteRendererComponent >( ) ) {
            entity.add< SpriteRendererComponent >( glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
        };
    };

    void SceneInspector::OnImGuiRender( Timestep::Timestep ts ) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
        if (ImGui::Begin(ICON_FA_CUBE "  Inspector")) {
            if( m_Context->m_selected_entity != entt::null ) {
                ImGuiTreeNodeFlags t = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
                
                Entity ent = { m_Context->m_selected_entity, m_Context };

                if( ent.has< TransformComponent >( ) ) {
                    auto name = ent.get< TagComponent >( );
                    ImGui::Text("Component name: %s", name.tag.c_str( ) );
                };

                if( ent.has< TransformComponent >( ) ) {
                    auto &[pos, scale, rot] = ent.get< TransformComponent >( );

                    if(ImGui::TreeNodeEx( " " ICON_FA_CUBES "  Transform", t)) {
                        imgui_gizmo_transform("Position", pos);
                        imgui_gizmo_transform("Rotation", rot);
                        imgui_gizmo_transform("Scale", scale, 1.0f);
                        
                        ImGui::TreePop();
                    };
                };

                if( ent.has< SpriteRendererComponent >( ) ) {
                    auto& sprite = ent.get< SpriteRendererComponent >( );

                    if(ImGui::TreeNodeEx(" " ICON_FA_PAINT_BRUSH "  SpriteRenderer", t)) {
                        ImGui::ColorEdit4("Sprite Color", glm::value_ptr(sprite.color));
                         
                        if(sprite.sprite.get() == nullptr) {
                            if(ImGui::Button("Bake Texture")) {
                                sprite.sprite = Sprite2D::Create( "resources/textures/checkerboard.png" );
                            };
                        } else {
                            ImGui::DragFloat("Tiling", &sprite.tiling, 0.05f, 1.0f, 100.0f );
                            ImGui::Image( reinterpret_cast< ImTextureID >( sprite.sprite->GetSprite( ) ),
                                ImVec2(64.0f, 64.0f ) 
                            );

                            if(ImGui::BeginDragDropTarget()) {
                                if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_EXPLORER_DRAG_DROP") ) {
                                    const char* sz_output = ( const char* )payload->Data;
                                    sprite.sprite->Change( sz_output );
                                }
                                ImGui::EndDragDropTarget();
                            };

                            if(ImGui::Button("Delete texture")) {
                                sprite.sprite.reset();
                            }
                        };
                        
                        ImGui::TreePop();
                    };
                };

                if(ImGui::BeginPopupContextWindow("##AddComponent", 1, false)) {
                    OnImGuiPopulateComponents(ent);
                    ImGui::EndPopup();
                };
            };
            ImGui::End();
        };
        ImGui::PopStyleVar();

        ImGui::ShowDemoWindow();

        if( ImGui::BeginMainMenuBar()) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
            if(ImGui::BeginMenu("File")) {
                if(ImGui::MenuItem("New", "CTRL+N")) {

                };
                if(ImGui::MenuItem("Save", "CTRL+S")) {
                    nlohmann::json data = nlohmann::json::array();

                    m_Context->m_register.each([&](auto entity_id ){
                        Entity ent = { entity_id, m_Context };
                            if(!ent) // ent = 0
                                return;
                            SerializeEntity(ent, static_cast<uint32_t>(entity_id), data);
                        });
                        
                    Viper::Util::JSONFileHandler().Write("test.json", data);
                };
                if(ImGui::MenuItem("Open", "CTRL+O")) {

                    // Before we load in a scene we want to clear the scene from all objects first.
                    m_Context->m_register.clear();

                    nlohmann::json data = Viper::Util::JSONFileHandler().Read("test.json");

                    for(int i = 0; i != data.size(); ++i ) {
                        auto objects = data[i][1];

                        if(!objects["Transform"]["Position"].is_null())
                            std::cout << Objects["Transform"]["Position"] << std::endl;
                    }
                };
                ImGui::EndMenu();
            };
            ImGui::PopStyleVar();
            ImGui::EndMainMenuBar();
        };
    };
};