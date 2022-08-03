#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // ortho

namespace Viper {
    class Renderer3D {
    public:
        static void Begin( const void* buffer_object );
    };
};