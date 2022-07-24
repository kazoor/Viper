#pragma once
#include <cstddef>
#include <functional>
#define VIPER_COMPONENT_DEF( x ) #x

#define VIPER_CLASS_DECLARATION( classname ) \
public: \
    bool IsComponentType( const std::size_t classType ) const; \
    static const std::size_t Type; \


#define VIPER_CLASS_DEFINE( ParentClass, ChildClass ) \
const std::size_t ChildClass::Type = std::hash< std::string >( )( VIPER_COMPONENT_DEF( ChildClass ) ); \
bool ChildClass::IsComponentType( const std::size_t classType ) const {  \
    if( classType == Type ) \
        return true; \
    return ParentClass::IsComponentType( classType ); \
}; \

namespace Viper::Components {
    class Component {
    public:
        virtual void OnAwake() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnEditor() = 0;

        virtual bool IsComponentType( const std::size_t compType ) const {
            return compType == Type;
        };
        
        static const std::size_t Type;
    };
};