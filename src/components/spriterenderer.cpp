#include <ImGui/imgui.h>
#include <memory>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "../util/globals/global.hpp"

namespace Viper::Components {
    VIPER_CLASS_DEFINE(Component, SpriteRenderer)

    SpriteRenderer::SpriteRenderer() {
        object = nullptr;
        color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    };

    SpriteRenderer::SpriteRenderer(GameObject* obj ) {
        object = obj;
        color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    };

    SpriteRenderer::SpriteRenderer(GameObject* obj, glm::vec4 col ) {
        object = obj;
        color = col;
    };

    void SpriteRenderer::OnAwake() {
    };

    void SpriteRenderer::OnUpdate(double deltatime) {
        tr = object->GetComponent< Transform >( );
        Globals::GlobalsContext::Renderer2D->DrawQuadRotated(
        glm::vec2(tr.position.x, tr.position.y),
        glm::vec2(tr.scale.x, tr.scale.y),
        tr.rotation.z,
        RendererAPI::Color(color.x, color.y, color.z, color.w));
    };

    void SpriteRenderer::OnEditor() {
        ImGuiTreeNodeFlags t = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
        if(ImGui::TreeNodeEx("SpriteRenderer", t)) {
            ImGui::ColorEdit4("Sprite Color", glm::value_ptr(color));
            ImGui::TreePop();
        };
    };
};