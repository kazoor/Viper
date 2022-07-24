#pragma once
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "component.hpp"

namespace Viper::Components {
    class GameObject {
    public:
        GameObject();
        GameObject( const std::string& tagname );
        std::string tag;

        template< class T = Component, typename... TArgs >
        void AddComponent( TArgs&&... args ) {
            m_Components.push_back( std::make_unique< T >( std::forward< TArgs >( args )... ) );
        };

        template< class T = Component >
        T& GetComponent( ) {
            for( auto&& component : m_Components ) {
                if( component->IsComponentType( T::Type ) ) {
                    return *static_cast< T* >( component.get( ) );
                };
            };
            return *std::unique_ptr< T >( nullptr );
        };

        template< class T = Component >
        bool RemoveComponent() {
            if( m_Components.empty() )
                return false;

            auto index = std::find_if( 
                m_Components.begin(),
                m_Components.end(), 
                [ classType = T::Type ]( auto& c) {
                    return c->IsComponentType( classType );
                });

            bool ret = ( index != m_Components.end() );

            if( ret )
                m_Components.erase( index );
            
            return ret;
        };

        template< class T = Component >
        bool HasComponent() {
            if( m_Components.empty() )
                return false;

            auto index = std::find_if(
                m_Components.begin(),
                m_Components.end(),
                [ classType = T::Type ]( auto& c ) {
                    return c->IsComponentType( classType );
                });


            bool component_found = ( index != m_Components.end() );

            return component_found;
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

        std::vector< std::unique_ptr< Component > > m_Components;
    };
};