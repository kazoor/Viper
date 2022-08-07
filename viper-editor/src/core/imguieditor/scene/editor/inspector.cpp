#include "inspector.hpp"

#include <iostream>
#include <imguieditor/fontawesome5.hpp>
#include <ImGui/imgui.h>
#include <unordered_map>
#include <string>
#include <scene/entitycomponents.hpp>
#include <scene/sceneentity.hpp>
#include <scene/scene.hpp>

#include <components/transform.hpp>
#include <components/rigidbody2d.hpp>
#include <components/boxcollision2d.hpp>

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

constexpr const char* RigidbodyTypes[] = { "Static", "Dynamic", "Kinematic" };
constexpr const char* CameraTypes[] = { "Perspective", "Orthographic" };
constexpr const char* SpriteTypes[] = { "Quad", "Triangle" };

namespace Viper {
    SceneInspector::SceneInspector( Scene* SceneContext ) : m_Context( SceneContext ) { };
    static std::string m_Nullified = "";

    void SerializeEntity( Entity entity, std::uint32_t entity_id, nlohmann::json& data ) {
        TransformComponent tr;
        SpriteRendererComponent sr;
        Rigidbody2DComponent rb2d;
        BoxCollider2DComponent bc2d;

        if(entity.has<TransformComponent>()) {
            tr = entity.get<TransformComponent>();
            data.push_back({
                entity_id,
                {
                    {"Tag", entity.get< TagComponent >( ).tag.c_str( )},
                    {"Transform", {
                        {"Position", {
                            {"X", tr.Translation.x},
                            {"Y", tr.Translation.y},
                            {"Z", tr.Translation.z}
                        }},
                        {"Scale", {
                            {"X", tr.Scale.x},
                            {"Y", tr.Scale.y},
                            {"Z", tr.Scale.z}
                        }},
                        {"Rotation", {
                            {"X", tr.Rotation.x},
                            {"Y", tr.Rotation.y},
                            {"Z", tr.Rotation.z}
                        }}
                 }},
            }});
        } // End transform

        if(entity.has<SpriteRendererComponent>()) {
            sr = entity.get<SpriteRendererComponent>();
            if( sr.sprite.get() != nullptr ) {
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
            } else {
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
                            }}
                        }}
                    }
                });
            }
        } // End spriterenderer

        if(entity.has<Rigidbody2DComponent>()) {
            rb2d = entity.get<Rigidbody2DComponent>();
            data.push_back({
                entity_id,
                {
                    {"Tag", entity.get< TagComponent >( ).tag.c_str( )},
                    {"Rigidbody2D", {
                        {"Type", static_cast<int>(rb2d.Type)},
                        {"FixedRotation", rb2d.FixedRotation},
                        {"Gravity", rb2d.Gravity}
                    }}
                }
            });
        } // End rigidbody2d 

        if(entity.has<BoxCollider2DComponent>()) {
            bc2d = entity.get<BoxCollider2DComponent>();
            data.push_back({
                entity_id,
                {
                    {"Tag", entity.get< TagComponent >( ).tag.c_str( )},
                    {"BoxCollider2D",{
                        {"Offset", {
                            {"X", bc2d.offset.x},
                            {"Y", bc2d.offset.y}
                        } },
                        {"Size", {
                            {"X", bc2d.size.x},
                            {"Y", bc2d.size.y}
                        }},
                        {"Density", bc2d.density},
                        {"Friction", bc2d.friction},
                        {"Restitution", bc2d.restitution},
                        {"RestitutionThreshold", bc2d.restitutionthreshold}
                    }}
                }
            });
        } 
    }

    void SceneInspector::OnImGuiPopulateComponents( Entity entity ) {
        OnImGuiPopulateContext< SpriteRendererComponent >( entity, "SpriteRenderer" );
        OnImGuiPopulateContext< Rigidbody2DComponent >( entity, "Rigidbody2D" );
        OnImGuiPopulateContext< BoxCollider2DComponent >( entity, "BoxCollider2D" );
        OnImGuiPopulateContext< CameraComponent >( entity, "Camera" );
    };

    template< typename T >
    void SceneInspector::OnImGuiPopulateContext( Entity ent, const std::string& name ) {
        if(!ent.has< T >( ) )
            if( ImGui::MenuItem( name.c_str( ) ) )
                ent.add< T >( );
    };

    void SceneInspector::OnImGuiRender( Timestep::Timestep ts ) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
        if (ImGui::Begin(ICON_FA_CUBE "  Inspector")) {
            if( m_Context->m_selected_entity != entt::null ) {
                ImGuiTreeNodeFlags t = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
                
                Entity ent = { m_Context->m_selected_entity, m_Context };
                static char tag_buffer[ 32 ] = { '\0' };
                if( ent.has< TransformComponent >( ) ) {
                    auto& name = ent.get< TagComponent >( );
                    ImGui::Text("Component name: %s", name.tag.c_str( ) );
                    ImGui::InputText("##comp_new_name", tag_buffer, 32 );
                    ImGui::SameLine();
                    if(ImGui::Button("Set name")) {
                        name.tag = tag_buffer;
                        tag_buffer[0] = '\0';
                    };
                };

                //if( m_Context->m_SceneState == SceneStates::State_Simulating )
                //    ImGui::BeginDisabled();

                // Camera Component [START].
                if( ent.has< TransformComponent >( ) ) {
                    auto &[pos, rot, scale] = ent.get< TransformComponent >( );

                    if(ImGui::TreeNodeEx( " " ICON_FA_CUBES "  Transform", t)) {
                        imgui_gizmo_transform("Position", pos);
                        imgui_gizmo_transform("Rotation", rot);
                        imgui_gizmo_transform("Scale", scale, 1.0f);
                        
                        ImGui::TreePop();
                    };
                };
                // Camera Component [END].

                // Camera Component [START].
                if( ent.has< CameraComponent >( ) ) {
                    auto &[c, mc, as] = ent.get< CameraComponent >( );

                    if(ImGui::TreeNodeEx( " " ICON_FA_CAMERA "  Camera", t)) {
                        ImGui::Checkbox("Main Camera", &mc);        
                        ImGui::Checkbox("Fixed Aspect Ratio", &as);
                        //const char* current_body_type = CameraTypes[(int)c.GetProjectionType()];
                        //if(ImGui::BeginCombo("Body type", current_body_type)) {
                        //    for( int i = 0; i < 2; i++ ) {
                        //        bool is_selected = current_body_type == CameraTypes[i];
                        //        if( ImGui::Selectable(CameraTypes[i], is_selected)) {
                        //            current_body_type = CameraTypes[i];
                        //            //rb2d.Type = (Rigidbody2DComponent::BodyType)i;
                        //            c.SetProjectionType((SceneCamera::ProjectionType)i);
                        //        };
//
                        //        if( is_selected )
                        //            ImGui::SetItemDefaultFocus();
                        //    }
                        //    ImGui::EndCombo();
                        //}; 

                        //if( c.GetProjectionType() == SceneCamera::ProjectionType::Orthographic ) {
                        //        auto _size = c.GetOrthographicSize();
                        //        if( ImGui::DragFloat("Ortho Size", &_size ) )
                        //            c.SetOrthographicSize(_size);
//
                        //        auto _near = c.GetOrthographicNear();
                        //        if( ImGui::DragFloat("Ortho Near", &_near ) )
                        //            c.SetOrthographicNear(_near);
//
                        //        auto _far = c.GetOrthographicFar();
                        //        if( ImGui::DragFloat("Ortho Far", &_far ) )
                        //            c.SetOrthographicFar(_far);
                        //    };
                        ImGui::TreePop();
                    };
                };
                // Camera Component [END].

                // Rigidbody2D Component [START].
                if( ent.has< Rigidbody2DComponent >( ) ) {
                    auto& rb2d = ent.get< Rigidbody2DComponent >( );
                    if(ImGui::TreeNodeEx( " " ICON_FA_CUBES "  Rigidbody2D", t)) {
                        if( ImGui::Button("Change to dynamic"))
                            rb2d.Type = Rigidbody2DComponent::BodyType::body_dynamic;

                        const char* current_body_type = RigidbodyTypes[(int)rb2d.Type];
                        if(ImGui::BeginCombo("Body type", current_body_type)) {
                            for( int i = 0; i < 2; i++ ) {
                                bool is_selected = current_body_type == RigidbodyTypes[i];
                                if( ImGui::Selectable(RigidbodyTypes[i], is_selected)) {
                                    current_body_type = RigidbodyTypes[i];
                                    rb2d.Type = (Rigidbody2DComponent::BodyType)i;
                                };

                                if( is_selected )
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        };

                        ImGui::Checkbox("Fixed Rotation", &rb2d.FixedRotation);
                        if(ImGui::Button("Add Force")) {
                            b2Body* body = ( b2Body* )rb2d.Rigidbody;
                            auto pos = glm::vec2(0.0f, 1.0f);
                            auto point = glm::vec2(0.0f, 0.0f);

                            body->ApplyForce({ pos.x, pos.y },{0.0f, 0.0f}, true);
                        }

                        if(ImGui::Button("Apply force")) {
                            b2Body* body = (b2Body*)rb2d.Rigidbody;
                            body->ApplyForce(b2Vec2(100.0f, 100.0f), b2Vec2(0.0f, 0.0f), true); 
                        };
                        ImGui::TreePop();
                    };
                };
                // Rigidbody2D Component [END].

                // BoxCollider2D Component [START].
                if( ent.has< BoxCollider2DComponent >( ) ) {
                    auto& box2d = ent.get< BoxCollider2DComponent >( );
                    if(ImGui::TreeNodeEx( " " ICON_FA_CUBES "  BoxCollider2D", t)) {
                        ImGui::DragFloat2("Offset", glm::value_ptr(box2d.offset));
                        ImGui::DragFloat2("Size", glm::value_ptr(box2d.size));
                        ImGui::DragFloat("Density", &box2d.density);
                        ImGui::DragFloat("Friction", &box2d.friction);
                        ImGui::DragFloat("Restitution", &box2d.restitution);
                        ImGui::DragFloat("Restitution Threshold", &box2d.restitutionthreshold);
                        ImGui::TreePop();
                    };
                };
                // BoxCollider2D Component [STOP].

                // SpriteRenderer Component [START].
                if( ent.has< SpriteRendererComponent >( ) ) {
                    auto& sprite = ent.get< SpriteRendererComponent >( );

                    if(ImGui::TreeNodeEx(" " ICON_FA_PAINT_BRUSH "  SpriteRenderer", t)) {
                        ImGui::ColorEdit4("Sprite Color", glm::value_ptr(sprite.color));

                        const char* current_sprite_type = SpriteTypes[(int)sprite.type];
                        if(ImGui::BeginCombo("Sprite Type", current_sprite_type)) {
                            for( int i = 0; i < 2; i++ ) {
                                bool is_selected = current_sprite_type == SpriteTypes[i];
                                if( ImGui::Selectable(SpriteTypes[i], is_selected)) {
                                    current_sprite_type = SpriteTypes[i];
                                    sprite.type = (SpriteRendererComponent::SpriteType)i;
                                };

                                if( is_selected )
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        };
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
                // SpriteRenderer Component [END].

                //if( m_Context->m_SceneState == SceneStates::State_Simulating )
                //    ImGui::EndDisabled();

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
                    m_Context->m_register.clear();
                };
                if(ImGui::MenuItem("Save", "CTRL+S")) {
                    //nlohmann::json data = nlohmann::json::array();
                    nlohmann::json data;

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

                    if(data.is_null()) {
                        VIPER_LOG("Failed to load file.");
                        return;
                    }
                    
                    for(int i = 0; i != data.size(); ++i ) {
                        // Make sure we dont hadd the same game object more than once
                        std::uint32_t id = data[i][0].get<std::uint32_t>();
                        auto component = data[i][1];
                        Viper::Entity ent;

                        std::unordered_map<std::uint32_t, std::string> objMap;
                        static int lastId = -1;
                        if(id != lastId) {
                            // Map up all the tags to their matching object id & create the entity.
                            objMap[id] = component["Tag"];

                            ent = m_Context->CreateEntity(component["Tag"]);

                            // Make sure to load the correct transform data.
                            auto& tr = ent.get<TransformComponent>();
                            tr.Translation = glm::vec3(component["Transform"]["Position"]["X"].get<float>(),
                                    component["Transform"]["Position"]["Y"].get<float>(),
                                    component["Transform"]["Position"]["Z"].get<float>());

                            tr.Scale = glm::vec3(component["Transform"]["Scale"]["X"].get<float>(),
                                    component["Transform"]["Scale"]["Y"].get<float>(),
                                    component["Transform"]["Scale"]["Z"].get<float>());

                            tr.Rotation = glm::vec3(component["Transform"]["Rotation"]["X"].get<float>(),
                                    component["Transform"]["Rotation"]["Y"].get<float>(),
                                    component["Transform"]["Rotation"]["Z"].get<float>());

                            lastId = id;
                        }

                        for(const auto &objects : objMap) {
                            const auto [mappedId, tag] = objects;
                            for(int j = 0; j != data.size(); j++) {
                                if(mappedId == data[j][0].get<int>()) {
                                   // Check for all the components each game object should have and add them.
                                   auto component = data[j][1];

                                   if(!component["SpriteRenderer"].is_null()) {
                                        ent.add<SpriteRendererComponent>();
                                        auto& sr = ent.get<SpriteRendererComponent>();

                                        sr.color = glm::vec4(
                                                component["SpriteRenderer"]["Color"]["R"].get<float>(),
                                                component["SpriteRenderer"]["Color"]["G"].get<float>(),
                                                component["SpriteRenderer"]["Color"]["B"].get<float>(),
                                                component["SpriteRenderer"]["Color"]["A"].get<float>()
                                            );
                                        
                                        if(component["SpriteRenderer"].contains("Sprite")) {
                                            sr.tiling = component["SpriteRenderer"]["Sprite"]["Tiling"].get<float>();
                                            sr.sprite = Sprite2D::Create(component["SpriteRenderer"]["Sprite"]["Path"].get<std::string>());
                                        }

                                        ent.get<SpriteRendererComponent>() = sr;
                                   }

                                   if(!component["Rigidbody2D"].is_null()) {
                                        ent.add<Rigidbody2DComponent>();
                                        auto& rigidbody = ent.get<Rigidbody2DComponent>();

                                        rigidbody.Type = static_cast<Viper::Rigidbody2DComponent::BodyType>(component["Rigidbody2D"]["Type"].get<int>());
                                        rigidbody.FixedRotation = component["Rigidbody2D"]["FixedRotation"].get<bool>();
                                        rigidbody.Gravity = component["Rigidbody2D"]["Gravity"].get<float>();
                                   }

                                    if(!component["BoxCollider2D"].is_null()) {
                                        ent.add<BoxCollider2DComponent>();
                                        auto& boxcollider = ent.get<BoxCollider2DComponent>();

                                        std::cout << component << std::endl;
                                        boxcollider.offset = glm::vec2(component["BoxCollider2D"]["Offset"]["X"].get<float>(), component["BoxCollider2D"]["Offset"]["Y"].get<float>());
                                        boxcollider.size = glm::vec2(component["BoxCollider2D"]["Size"]["X"].get<float>(), component["BoxCollider2D"]["Size"]["Y"].get<float>());
                                        boxcollider.density = component["BoxCollider2D"]["Density"].get<float>();
                                        boxcollider.friction = component["BoxCollider2D"]["Friction"].get<float>();
                                        boxcollider.restitution = component["BoxCollider2D"]["Restitution"].get<float>();
                                        boxcollider.restitutionthreshold = component["BoxCollider2D"]["RestitutionThreshold"].get<float>();
                                    }
                                }
                            }
                        }
                    }
                };
                ImGui::EndMenu();
            };
            ImGui::PopStyleVar();
            ImGui::EndMainMenuBar();
        };
    };
};