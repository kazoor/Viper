#include "hierarchy.hpp"

#include <graphics/renderer/renderer.hpp>
#include <imguieditor/fontawesome5.hpp>
#include <ImGui/imgui.h>

#include <scene/entitycomponents.hpp>
#include <scene/sceneentity.hpp>
#include <scene/scene.hpp>

namespace Viper {
    static char buff[80] = { '\0' };
    SceneHierarchy::SceneHierarchy( Scene* SceneContext ) : m_Context( SceneContext ) { };

    void SceneHierarchy::OnImGuiRender( Timestep::Timestep ts ) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

        if (ImGui::Begin(ICON_FA_SITEMAP "  Hierarchy") ) {
            ImGui::Text("engine fps: %.2f, average %.3fms/frame", 1.0f / ts.seconds(), ts.milliseconds());

            OnImGuiRenderItems( ts );
            if( ImGui::BeginPopupContextWindow(0, 1, false)) {
                if(ImGui::MenuItem("Create Empty Entity"))
                {
                    auto ent = m_Context->CreateEntity( "hentai maker" );
                    Entity entity = { ent, m_Context };
                    ent = entity;
                }
                ImGui::EndPopup();
            };
            ImGui::End();
        };
        ImGui::PopStyleVar();
    };

    void SceneHierarchy::OnImGuiRenderItems( Timestep::Timestep ts ) {
        auto view = m_Context->m_register.view< Viper::tag_t >( );

        for( auto entity : view ) {
            auto tag_component = view.get< Viper::tag_t >( entity );

            ImGuiTreeNodeFlags flag = ( ( m_Context->m_selected_entity == entity ) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
            bool is_open = ImGui::TreeNodeEx( tag_component.tag.c_str( ), flag, tag_component.tag.c_str( ) );

            if (ImGui::IsItemClicked())
                m_Context->m_selected_entity = entity;

            if (is_open) {
                if (ImGui::Button(std::string(ICON_FA_TRASH_ALT " Remove ").append(tag_component.tag).c_str())) {
                    m_Context->DestroyEntity( entity );
                    m_Context->m_selected_entity = entt::null;
                }
                ImGui::TreePop();
            };
        };
    };
};