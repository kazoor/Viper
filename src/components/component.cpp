#include "component.hpp"

namespace Viper::Components {
    const std::size_t Component::Type = std::hash< std::string >( )( VIPER_COMPONENT_DEF( Component ) );
};