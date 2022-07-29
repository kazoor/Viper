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

    void GameObject::OnDeletion() {
        for( auto&& c = components.rbegin(); c != components.rend(); c++ ) {
            if( c->get()->GetComponentType() == ComponentType::Transform )
                continue;
                
            if( ImGui::Button( VIPER_FORMAT_STRING( ICON_FA_TRASH_ALT " Remove %s", c->get()->GetName() ).c_str( ) ) ) {
                RemoveComponent( c->get()->GetName());
            };
        };
    }

    void GameObject::OnEditor() {
        for( auto&& c : components ) {
            c->Editor( );
        };
    };

    void GameObject::OnAwake() {
        for(auto& c : components )
            c->Awake( );
    };

    void GameObject::OnUpdate( double deltaticks ) {
        for(auto& c : components )
            c->Update( deltaticks );
    };

    size_t GameObject::GetComponents( ) const {
        return components.size();
    };

     const char* GameObject::GetComponentNameByIndex( uint32_t index ) {
        if( components.empty() )
            return nullptr;

        return components.at( index )->GetName( );
     };
};