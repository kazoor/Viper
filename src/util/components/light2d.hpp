#pragma once
#include "component.hpp"

class Light2D : public Component {
public:
    Light2D() {
        // setup
    };

    void OnUpdate( const float elapsed ) override {
        transform.position += glm::vec3( 1.0f );
    };
};