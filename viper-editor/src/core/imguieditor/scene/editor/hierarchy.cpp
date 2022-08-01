#include "hierarchy.hpp"

#include <graphics/renderer/renderer.hpp>
#include <imguieditor/fontawesome5.hpp>
#include <ImGui/imgui.h>

namespace Viper::Editor {
    static char buff[80] = { '\0' };
    SceneHierarchy::SceneHierarchy( const Ref< Scene >& SceneContext ) : m_Context( SceneContext ) { };

    void SceneHierarchy::OnImGuiRender( Timestep::Timestep ts ) {

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

        if (ImGui::Begin(ICON_FA_SITEMAP "  Hierarchy")) {
            
            ImGui::End();
        };
        ImGui::PopStyleVar();
        
    };
};