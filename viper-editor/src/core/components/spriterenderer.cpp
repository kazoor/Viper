#include <ImGui/imgui.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "../util/globals/global.hpp"
#include "../imguieditor/fontawesome5.hpp"
#include "spriterenderer.hpp"

namespace Viper::Components {
    SpriteRenderer::SpriteRenderer() {
        object = nullptr;
        color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        enabled = true;
    };

    SpriteRenderer::SpriteRenderer(GameObject* obj ) {
        object = obj;
        color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        enabled = true;
    };

    SpriteRenderer::SpriteRenderer(GameObject* obj, glm::vec4 col ) {
        object = obj;
        color = col;
        enabled = true;
    };

    void SpriteRenderer::OnUpdate(Timestep::Timestep ts) {
        tr = object->GetComponent< Transform >( );
        if(!enabled)
            return;

        
    };
};