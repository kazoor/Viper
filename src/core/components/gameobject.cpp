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
        for(auto&& c = components.rbegin(); c != components.rend(); c++ ) {
            if( c->get()->GetComponentType() == ComponentType::Transform )
                continue;

            RemoveComponent(c->get()->GetName());
        };
    };

    Ref< GameObject > GameObject::Create( ) {
        return CreateRef< GameObject >( );
    };

    Ref< GameObject > GameObject::Create( const std::string& tagname ) {
        return CreateRef< GameObject >( tagname );
    };
};