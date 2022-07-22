#pragma once
#include <glm/vec3.hpp>
#include "color.hpp"

namespace Viper::RendererAPI {
    struct Vertex_t {
        glm::vec3 position;
        Color color;
    };
};