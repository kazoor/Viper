#pragma once
#include "input.hpp"
#include "../entity.hpp"

namespace Viper::Components {
    class DemoComponent : public Entity {
    public:
        DemoComponent();

        void OnUpdate() override;
    private:
        Components::InputAPI* input;
    };
};