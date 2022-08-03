#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "component.hpp"
#include "gameobject.hpp"


namespace Viper::Components {
    class SpriteRenderer : public Component {
    public:
        VIPER_COMPONENT_DECLARE( SpriteRenderer );

        SpriteRenderer();
        
        SpriteRenderer(GameObject* obj);

        SpriteRenderer(GameObject* obj, glm::vec4 col );
        
        void OnUpdate(Timestep::Timestep ts);

        glm::vec4 color;
    private:
        GameObject* object;
    protected:
        Transform tr;
    };
}