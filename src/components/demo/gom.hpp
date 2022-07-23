#pragma once
#include <vector>
#include <memory>
#include "../gameobject.hpp"
#include "../transform.hpp"
#include "../game.hpp"
namespace Viper::Components {
    class GameObjectManager : public GameObject {
    public:
        void OnUpdate();
        void OnAwake();
        void OnAdd( std::unique_ptr< GameObject > gameObject );
    private:
        std::vector< std::unique_ptr< GameObject > > m_GameObjects;
    };
};