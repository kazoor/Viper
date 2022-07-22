#pragma once

#include "../../layers/layer/layer.hpp"
#include "../../events/event/event.hpp"
#include "../../graphics/window/window.hpp"

namespace Viper::ImGuiEditor::Scene {
    class Scene : public Viper::Layers::Layer {
    public:
        Scene() : Layer("Editor Scene") {}

        void OnUpdate() override {

        }

        void OnEvent(Viper::Events::Event *Event) override {

        }
    };
}