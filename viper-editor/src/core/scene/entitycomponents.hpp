#pragma once
#include <glm/vec3.hpp>
#include <string.h>

namespace Viper {
    struct transform_t {
        transform_t() = default;
        transform_t( const transform_t& o ) = default;
        transform_t( const glm::vec3& position ) : pos( position ) { };
        glm::vec3 pos;
    };

    struct tag_t {
        tag_t() = default;
        tag_t( const tag_t& o ) = default;
        tag_t( const std::string& tagname ) : tag( tagname ) { };

        std::string tag;
    };
};