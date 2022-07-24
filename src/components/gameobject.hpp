#pragma once
#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include "component.hpp"

namespace Viper::Components {
    class GameObject {
    public:
        GameObject();
        GameObject( const std::string& tagname );
        std::string tag;

        template< typename T = Component, typename... TArgs >
        inline void AddComponent( TArgs&&... args ) {
            m_Components.push_back( std::make_unique< T >( std::forward< TArgs >( args )... ) );
        };

        template< class T >
        T& GetComponent( ) {
            for( auto&& component : m_Components ) {
                if( component->IsComponentType( T::Type ) ) {
                    return *static_cast< T* >( component.get( ) );
                };
            };
            return *std::unique_ptr< T >( nullptr );
        };

        void OnUpdate() {
            for( auto& c : m_Components )
                c->OnUpdate();
        };

        void OnAwake() {
            for( auto& c : m_Components )
                c->OnAwake();
        };

        void OnEditor() {
            for( auto& c : m_Components )
                c->OnEditor();
        };
    private:
        std::vector< std::unique_ptr< Component > > m_Components;
    };
};