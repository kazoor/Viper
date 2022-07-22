#pragma once
#include "entity.hpp"
#include <memory>
#include <vector>
namespace Viper::Components {
    class EntityManager {
    public:
        void AddEntity( std::unique_ptr< Entity > entity );

        void AddUpdate();

        void AddGui();
    private:
        using ref_t = std::vector< std::unique_ptr< Entity > >;
        ref_t m_Entities;
    };
};