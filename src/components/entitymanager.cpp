#include "entitymanager.hpp"
#include <spdlog/spdlog.h>

namespace Viper::Components {
    void EntityManager::AddEntity( std::unique_ptr< Entity > entity ) {
        m_Entities.push_back( std::move( entity ) );
    };

    void EntityManager::AddUpdate() {
        for( auto& e : m_Entities )
            e->OnUpdate();
    };

    void EntityManager::AddGui() {
        for( auto& e : m_Entities )
            e->OnGui();
    };
};