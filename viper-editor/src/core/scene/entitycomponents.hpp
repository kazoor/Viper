#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <graphics/renderer/sprite2d.hpp>
#include <graphics/renderer/camera/scene_camera.hpp>
#include <util/ref/reference.hpp>
#include <viper/viper.hpp>
#include <string.h>
#include <box2d/include/box2d/b2_body.h>

#define VIPER_INCOMPLETE_COMP( CompDef ) struct CompDef { \
    CompDef() { printf("error, comp not complete for " #CompDef ); } \
    CompDef( const CompDef& o ) = default; \
    \
};

namespace Viper {
    struct VIPER_API TransformComponent {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
    };

    struct VIPER_API TagComponent {
        std::string tag = "{un_named_object}";

        TagComponent() = default;
        TagComponent( const TagComponent& o ) = default;
        TagComponent( const std::string& tagname ) : tag( tagname ) { };
    };

    struct VIPER_API SpriteRendererComponent {
        enum class SpriteType : int {
            sprite_quad,
            sprite_triangle
        };

        glm::vec4 color = glm::vec4( 1.0f );
        float tiling = 1.0f;
        Ref< Sprite2D > sprite = nullptr;
        SpriteType type = SpriteType::sprite_quad;

        SpriteRendererComponent() = default;
        SpriteRendererComponent( const SpriteRendererComponent& o ) = default;
        SpriteRendererComponent( const glm::vec4& col ) : color( col ), sprite( ) { };
        SpriteRendererComponent( const glm::vec4& col, const Ref< Sprite2D >& texture ) : color( col ), sprite( texture ) { };
    };

    struct VIPER_API Rigidbody2DComponent {
        enum class BodyType : int {
            body_static = 0,
            body_dynamic,
            body_kinematic
        };

        BodyType Type = BodyType::body_static;
        bool FixedRotation = false;
        void* Rigidbody = nullptr;
        float Gravity = 1.0f;

        void AddForce( const glm::vec2& force, const glm::vec2& point ) const {
            ((b2Body*)Rigidbody)->ApplyForce( { force.x, force.y }, { point.x, point.y }, true );
        };

        void AddForceCenter(const glm::vec2& force_center) {
            ((b2Body*)Rigidbody)->ApplyForceToCenter({force_center.x,force_center.y}, true);
        }

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent( const Rigidbody2DComponent& ) = default;
    };

    struct VIPER_API BoxCollider2DComponent {
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

    struct VIPER_API CameraComponent {
        SceneCamera camera;
        bool MainCamera = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent( const CameraComponent& ) = default;
    };

    struct VIPER_API LightComponent {
        bool IsEnabled = true;

        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
    };

    VIPER_INCOMPLETE_COMP(NativeScriptComponent)
};