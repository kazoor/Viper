#include "shadereditor.hpp"
#include <ImGui/imgui.h>

namespace Viper {
    char buff[4096];
    void SceneShaderEditor::OnImGuiRender(Timestep::Timestep ts ) {
        if(ImGui::Begin("Shader Editor")) {
            ImGui::InputTextMultiline( "##Shader", buff, 4096 );
            ImGui::End();
        };
    };
};