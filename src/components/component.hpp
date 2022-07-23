#pragma once
#include <cstddef>
#include <functional>
#define VIPER_COMPONENT_DEF( x ) #x

namespace Viper::Components {
    class Component {
    public:
        virtual void OnAwake() = 0;
        virtual void OnUpdate() = 0;

        virtual bool IsComponentType( const std::size_t compType ) const {
            return compType == Type;
        };
        
        std::size_t Type;
    };
};