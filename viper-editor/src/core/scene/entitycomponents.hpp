#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string.h>

#define VIPER_INCOMPLETE_COMP( CompDef ) struct CompDef { \
    CompDef() { printf("error, comp not complete for " #CompDef ); } \
};

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

    struct spriterenderer_t {
        spriterenderer_t() = default;
        spriterenderer_t( const spriterenderer_t& o ) = default;
        spriterenderer_t( const glm::vec4& col ) : color( col ) { };

        glm::vec4 color;
    };

    VIPER_INCOMPLETE_COMP(rigidbody2d_t)
    VIPER_INCOMPLETE_COMP(boxcollider2d_t)
    VIPER_INCOMPLETE_COMP(camera_t)
    VIPER_INCOMPLETE_COMP(nativescript_t)
};