#include "gameobject.hpp"

namespace Viper::Components {
    GameObject::GameObject() : tag("undefined gameobject.") { };
    
    GameObject::GameObject( const std::string& tagname ) : tag( tagname ) { };

    Ref< GameObject > GameObject::Create( const std::string& tagname ) {
        return CreateRef< GameObject >( tagname );
    };
};