#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "color.hpp"

namespace Viper::RendererAPI {
    struct Vertex_t {
        glm::vec3 position;
        Color color;
        glm::vec2 texcoords;
        float texindex;
    };

    struct LineVertex_t {
        glm::vec3 pos;
        Color color;
    };
};