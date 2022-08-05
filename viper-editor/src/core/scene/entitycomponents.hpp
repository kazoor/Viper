#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <graphics/renderer/sprite2d.hpp>
#include <util/ref/reference.hpp>
#include <string.h>

#define VIPER_INCOMPLETE_COMP( CompDef ) struct CompDef { \
    CompDef() { printf("error, comp not complete for " #CompDef ); } \
    CompDef( const CompDef& o ) = default; \
    \
};

namespace Viper {
    struct TransformComponent {
        TransformComponent() = default;
        TransformComponent( const TransformComponent& o ) = default;
        TransformComponent( const glm::vec3& pos ) : position( pos ), scale(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)) { };
        TransformComponent( const glm::vec3& pos, const glm::vec3& size ) : position( pos ), scale(size), rotation(glm::vec3(0.0f)) { };
        TransformComponent( const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rot ) : position( pos ), scale(size), rotation(rot) { };
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
    };

    struct TagComponent {
        TagComponent() = default;
        TagComponent( const TagComponent& o ) = default;
        TagComponent( const std::string& tagname ) : tag( tagname ) { };

        std::string tag;
    };

    struct SpriteRendererComponent {
        SpriteRendererComponent() = default;
        SpriteRendererComponent( const SpriteRendererComponent& o ) = default;
        SpriteRendererComponent( const glm::vec4& col ) : color( col ), sprite( ) { };
        SpriteRendererComponent( const glm::vec4& col, const Ref< Sprite2D >& texture ) : color( col ), sprite( texture ) { };

        glm::vec4 color;
        float tiling = 1.0f;
        Ref< Sprite2D > sprite;
    };

    struct Rigidbody2DComponent {
        enum class BodyType : int {
            body_static = 0,
            body_dynamic,
            body_kinematic
        };

        BodyType Type = BodyType::body_static;
        bool FixedRotation = false;
        void* Rigidbody = nullptr;

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent( const Rigidbody2DComponent& ) = default;
    };

    struct BoxCollider2DComponent {
        glm::vec2 offset = { 0.0f, 0.0f };
        glm::vec2 size = { 0.5f, 0.5f };
        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.0f;
        float restitutionthreshold = 0.5f;
        void* RuntimeFixture = nullptr;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent( const BoxCollider2DComponent&) = default;
    };

    VIPER_INCOMPLETE_COMP(CameraComponent)
    VIPER_INCOMPLETE_COMP(NativeScriptComponent)
};