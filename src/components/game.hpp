#pragma once
#include "component.hpp"
#include <spdlog/spdlog.h>
namespace Viper::Components {
    class Game : public Component {
    public:
        Game() {
            Type = std::hash<std::string>()(VIPER_COMPONENT_DEF(Game));
        };
        bool IsComponentType( const std::size_t compType ) const {
            if( compType == Game::Type )
                return true;

            return Component::IsComponentType( compType );
        };

        void OnAwake() override {

        };
        void OnUpdate() override {
            spdlog::warn("hello from OnUpdate.");
        };
    };
};