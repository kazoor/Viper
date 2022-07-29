#pragma once
#include <vector>
#include <memory>
#include "gameobject.hpp"
#include "transform.hpp"
//#include "spriterenderer.hpp"
//#include "camera.hpp"
#include "../util/ref/reference.hpp"

namespace Viper::Components {
    class GameObjectManager : public GameObject {
    public:
        void OnUpdate( double deltatime );
        void OnAwake();
        void OnAdd( Ref< GameObject > gameobject );
        size_t GameObjectSize() { return m_GameObjects.size( ); };

        std::vector< Ref< GameObject > >::iterator begin() { return m_GameObjects.begin(); };
        std::vector< Ref< GameObject > >::iterator end() { return m_GameObjects.end(); };

        std::vector< Ref< GameObject > >::reverse_iterator rbegin() { return m_GameObjects.rbegin(); };
        std::vector< Ref< GameObject > >::reverse_iterator rend() { return m_GameObjects.rend(); };

        std::vector< Ref< GameObject > >::const_reverse_iterator rbegin() const { return m_GameObjects.rbegin(); };
        std::vector< Ref< GameObject > >::const_reverse_iterator rend() const { return m_GameObjects.rend(); };

        std::vector< Ref< GameObject > >::const_iterator begin() const { return m_GameObjects.begin(); };
        decltype(auto) get( const size_t index ) { return m_GameObjects.at( index ); };
        decltype(auto) pop( const size_t index ) { m_GameObjects.erase( m_GameObjects.begin( ) + index ); }; 
    private:
        std::vector< Ref< GameObject > > m_GameObjects;
    };
};