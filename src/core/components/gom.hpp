#pragma once
#include <vector>
#include <memory>
#include "gameobject.hpp"
#include "transform.hpp"
#include "spriterenderer.hpp"
#include "camera.hpp"

namespace Viper::Components {
    class GameObjectManager : public GameObject {
    public:
        void OnUpdate(double deltatime);
        void OnAwake();
        void OnAdd( std::unique_ptr< GameObject > gameObject );
        size_t GameObjectSize() { return m_GameObjects.size( ); };
        std::vector< std::unique_ptr< GameObject > > m_GameObjects;
    };
};