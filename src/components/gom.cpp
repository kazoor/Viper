#include "gom.hpp"

namespace Viper::Components {
    void GameObjectManager::OnUpdate() {
        for( auto& go : m_GameObjects )
            go->OnUpdate();
    };

    void GameObjectManager::OnAwake() {
        for( auto& go : m_GameObjects )
            go->OnAwake();
    };

    void GameObjectManager::OnAdd( std::unique_ptr< GameObject > gameObject ) {
        m_GameObjects.push_back( std::move( gameObject ) );
    };
};