#include "gameobject.hpp"
#include "transform.hpp"

namespace Viper::Components {
    GameObject::GameObject() : tag("{undefined gameobject}") {
        AddComponent< Transform >(
            glm::vec3( 0.0f, 0.0f, 0.0f ),
            glm::vec3( 1.0f, 1.0f, 2.0f ),
            glm::vec3( 0.0f, 0.0f, 0.0f )
        );
     };
    
    GameObject::GameObject( const std::string& tagname ) : tag( tagname ) {
        AddComponent< Transform >(
            glm::vec3( 0.0f, 0.0f, 0.0f ),
            glm::vec3( 1.0f, 1.0f, 2.0f ),
            glm::vec3( 0.0f, 0.0f, 0.0f )
        );
    };

    GameObject::~GameObject() {
        for( auto&& c : components ) // automatically deconstruct all the sub-components.
        {
            if(c->GetComponentType() == ComponentType::Transform)
                continue;

            RemoveComponent(c->GetName());
        };
            //RemoveComponent(c->GetName()); // this is great for debugging, but it should be handled by the user, and not the class.
    };

    Ref< GameObject > GameObject::Create( ) {
        return CreateRef< GameObject >( );
    };

    Ref< GameObject > GameObject::Create( const std::string& tagname ) {
        return CreateRef< GameObject >( tagname );
    };
};