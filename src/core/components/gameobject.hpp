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
                components.push_back( std::make_unique< T >( std::forward< TArgs >( args )... ) );
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
            if( found_in_table ) {
                printf("found component. has_component.\n");
                return true;
            } else {
                printf("error, can't find component. has_component. \n");
                return false;
            };
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
            return *std::unique_ptr< T >( nullptr );
        };

        const char* GetComponentNameByIndex( uint32_t index ) {
            if( components.empty() )
                return nullptr;

            return components.at( index )->GetName( );
        };

        size_t GetComponents() const {
            return components.size();
        };

        void OnUpdate( double deltaticks ) {
            for(auto& c : components )
                c->Update( deltaticks );
        };

        void OnAwake( ) {
            for(auto& c : components )
                c->Awake( );
        };

        void OnEditor( ) {
            for( auto&& c : components ) {
                c->Editor( );
            };
        };

        void OnDeletion() {
            for( auto&& c = components.rbegin(); c != components.rend(); c++ ) {
                if( c->get()->GetComponentType() == ComponentType::Transform )
                    continue;
                    
                if( ImGui::Button( VIPER_FORMAT_STRING( ICON_FA_TRASH_ALT " Remove %s", c->get()->GetName() ).c_str( ) ) )
                    RemoveComponent( c->get()->GetName());
            };
        };

        static Ref< GameObject > Create();
        static Ref< GameObject > Create(const std::string& tagname);

        std::string tag;
    private:
        std::vector< Scope< Component > > components;
    };
};