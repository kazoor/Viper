#include "gom.hpp"

namespace Viper::Components {
    void GameObjectManager::OnUpdate(double deltatime) {
        for( auto& go : m_GameObjects )
            go->OnUpdate(deltatime);
    };

    void GameObjectManager::OnAwake() {
        for( auto& go : m_GameObjects )
            go->OnAwake();
    };

    void GameObjectManager::OnAdd( Ref< GameObject > gameObject ) {
        m_GameObjects.push_back( gameObject );
    };
};