#pragma once
#include <glm/vec3.hpp>
#include "component.hpp"

namespace Viper::Components {
    class Transform : public Component {
        VIPER_CLASS_DECLARATION( Transform )
    public:
        Transform();
        Transform(glm::vec3 pos);
        Transform(glm::vec3 pos, glm::vec3 sc);
        Transform(glm::vec3 pos, glm::vec3 sc, glm::vec3 rot);

        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;

        void OnAwake() override;
        
        void OnUpdate(double deltatime) override;

        void OnEditor() override;
    };
};