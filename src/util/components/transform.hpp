#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform {
public:
    Transform() {
        position = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        scale = glm::vec3(0.0f);
        transform = glm::mat4(0.0f);
    };

    Transform( glm::vec3 pos ) {
        position = pos;
        rotation = glm::vec3(0.0f);
        scale = glm::vec3(0.0f);
        transform = glm::mat4(0.0f);
    };

    Transform( glm::vec3 pos, glm::vec3 size ) {
        position = pos;
        rotation = glm::vec3(0.0f);
        scale = size;
        transform = glm::mat4(0.0f);
    };

    Transform( glm::vec3 pos, glm::vec3 size, glm::vec3 rot ) {
        position = pos;
        rotation = rot;
        scale = size;
        transform = glm::mat4(0.0f);
    };

    Transform( glm::vec3 pos, glm::vec3 size, glm::vec3 rot, glm::mat4 trans ) {
        position = pos;
        rotation = rot;
        scale = size;
        transform = trans;
    };

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 transform;
};