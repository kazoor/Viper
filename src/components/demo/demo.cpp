#include "demo.hpp"
#include <spdlog/spdlog.h>

namespace Viper::Components {
    DemoComponent::DemoComponent() {
        tag = { "Hejsan!" };
        input = AddComponent< Components::InputAPI >( );
    };

    void DemoComponent::OnUpdate() {
        if( input->IsKeyDown(65)) {
            spdlog::info("DemoComponent::Update() :: Tag Name {:s}", tag );
        }
    };
};