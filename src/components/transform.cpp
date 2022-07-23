#include <string>
#include "transform.hpp"

namespace Viper::Components {
    Transform::Transform() {
        position = glm::vec3(0.0f);
        scale = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        Type = std::hash<std::string>()(VIPER_COMPONENT_DEF(Transform));
    };

    Transform::Transform( glm::vec3 pos ) {
        position = pos;
        scale = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        Type = std::hash<std::string>()(VIPER_COMPONENT_DEF(Transform));
    };

    Transform::Transform( glm::vec3 pos, glm::vec3 sc ) {
        position = pos;
        scale = sc;
        rotation = glm::vec3(0.0f);
        Type = std::hash<std::string>()(VIPER_COMPONENT_DEF(Transform));
    };

    Transform::Transform( glm::vec3 pos, glm::vec3 sc, glm::vec3 rot ) {
        position = pos;
        scale = sc;
        rotation = rot;
        Type = std::hash<std::string>()(VIPER_COMPONENT_DEF(Transform));
    };
};