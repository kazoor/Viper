#pragma once
#include <glm/vec3.hpp>
#include <string.h>

namespace Viper {
    struct transform_t {
        transform_t() = default;
        transform_t( const transform_t& o ) = default;
        transform_t( const glm::vec3& pos ) : position( pos ), scale(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)) { };
        transform_t( const glm::vec3& pos, const glm::vec3& size ) : position( pos ), scale(size), rotation(glm::vec3(0.0f)) { };
        transform_t( const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rot ) : position( pos ), scale(size), rotation(rot) { };
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
    };

    struct tag_t {
        tag_t() = default;
        tag_t( const tag_t& o ) = default;
        tag_t( const std::string& tagname ) : tag( tagname ) { };

        std::string tag;
    };
};