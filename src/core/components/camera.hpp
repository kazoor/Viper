#pragma once
#include <glm/vec3.hpp>
#include "component.hpp"
#include "transform.hpp"
#include "gameobject.hpp"

namespace Viper::Components {
    class Camera : public Component {
        VIPER_CLASS_DECLARATION( Camera )
    public:
        Camera();

        Camera(GameObject* parent);

        void OnAwake() override;
        
        void OnUpdate(double deltatime) override;

        void OnEditor() override;
    private:
        GameObject* object;
    };
};