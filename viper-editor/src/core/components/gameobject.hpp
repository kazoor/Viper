#pragma once
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>
#include "transform.hpp"
#include "component.hpp"
#include "../util/ref/reference.hpp"
#include "../imguieditor/fontawesome5.hpp"
#include <ImGui/imgui.h>
#include "../events/event/event.hpp"
#include "../viper/base.hpp"

namespace Viper::Components {
    class GameObject {
    public:
        GameObject();
        GameObject( const std::string& tagname );

        ~GameObject();

        template< class T, typename... TArgs >
        void AddComponent(TArgs&&... args) {
            auto idx = std::find_if(components.begin(), components.end(), [ a = T::GetType() ]( auto& c ) { return c->IsComponentValid( a ) && a != ComponentType::Scripting;});
            bool found_in_table = ( idx != components.end());

            if( !found_in_table ) // component already exist.
                components.push_back( CreateScope< T >( std::forward< TArgs >( args )... ) );
        };

        template< typename T >
        void RemoveComponent() {
            if(components.empty())
                return;

            auto idx = std::find_if(components.begin(), components.end(), [ a = T::GetType() ]( auto& c ) { return c->IsComponentValid(a);});
            bool found_in_table = ( idx != components.end());

            if( found_in_table )
                components.erase( idx );
        };

        void RemoveComponent( const char* name ) {
            if(components.empty())
                return;

            auto idx = std::find_if(components.begin(), components.end(), [ a = name ]( auto& c ) { return strstr(c->GetName(), a);});
            bool found_in_table = ( idx != components.end());

            if( found_in_table )
                components.erase( idx );
        };

        template< class T >
        bool HasComponent( ) {
            auto idx = std::find_if(components.begin(), components.end(), [ a = T::GetType() ]( auto& c ) { return c->IsComponentValid(a);});
            bool found_in_table = ( idx != components.end());
            return found_in_table;
        };

        bool HasComponent( const char* name ) {
            auto idx = std::find_if(components.begin(), components.end(), [ a = name ]( auto& c ) { return strstr(c->GetName(), a);});
            bool found_in_table = ( idx != components.end());
            return found_in_table;
        };

        template< class T >
        T& GetComponent() {
            for( auto&& c : components ) {
                if( c->IsComponentValid( T::GetType( ) ) ) {
                    return *static_cast< T* >( c.get( ) );
                };
            };
            return *Scope< T >( nullptr );
        };

        const char* GetComponentNameByIndex( uint32_t index );

        size_t GetComponents( ) const;

        void OnUpdate( Timestep::Timestep ts );

        void OnAwake( );

        void OnEditor( );

        void OnDeletion();

        void OnEvent(Viper::Events::Event& e);

        static Ref< GameObject > Create();
        static Ref< GameObject > Create(const std::string& tagname);

        std::string tag;
    private:
        std::vector< Scope< Component > > components;
    };
};