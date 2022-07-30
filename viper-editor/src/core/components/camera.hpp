#pragma once
#include <glm/vec3.hpp>
#include "component.hpp"
#include "gameobject.hpp"

namespace Viper::Components {
    class Camera : public Component {
    public:
        VIPER_COMPONENT_DECLARE( Camera );

        Camera();

        Camera(GameObject* parent);

        void Awake();
        
        void Update(Timestep::Timestep ts);

        void Editor();
    private:
        GameObject* object;
    };
};