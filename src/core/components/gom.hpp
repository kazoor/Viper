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
        void OnUpdate(double deltatime);
        void OnAwake();
        void OnAdd( Ref< GameObject > gameobject );
        size_t GameObjectSize() { return m_GameObjects.size( ); };
        std::vector< Ref< GameObject > > m_GameObjects;
    };
};