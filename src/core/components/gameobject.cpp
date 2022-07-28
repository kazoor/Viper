#include "gameobject.hpp"

namespace Viper::Components {
    GameObject::GameObject() : tag("undefined gameobject.") { };
    
    GameObject::GameObject( const std::string& tagname ) : tag( tagname ) { };
};