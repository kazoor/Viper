#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "../util/globals/global.hpp"

namespace Viper::Components {
    VIPER_CLASS_DEFINE(Component, SpriteRenderer)

    SpriteRenderer::SpriteRenderer() {
        object = nullptr;
    }
    SpriteRenderer::SpriteRenderer(GameObject* obj ) {
        object = obj;
        color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    };

    void SpriteRenderer::OnAwake() {

    };
        
    void SpriteRenderer::OnUpdate() {
        if( object->HasComponent< Transform >( ) ) {
            auto& tr = object->GetComponent< Transform >( );
            Globals::GlobalsContext::Renderer2D->DrawQuadRotated(
            glm::vec2(tr.position.x, tr.position.y),
            glm::vec2(tr.scale.x, tr.scale.y),
            tr.rotation.z,
            RendererAPI::Color(color.x, color.y, color.z, color.w));
        };
    };

    void SpriteRenderer::OnEditor() {
        ImGuiTreeNodeFlags t = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
        if(ImGui::TreeNodeEx("SpriteRenderer", t)) {
            ImGui::ColorEdit4("Sprite Color", glm::value_ptr(color));
            ImGui::TreePop();
        };
    };
};