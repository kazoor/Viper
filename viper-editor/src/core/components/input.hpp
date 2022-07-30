#pragma once

#include <glm/vec4.hpp>
#include <map>
#include <utility>
#include <memory>
#include "transform.hpp"
#include "component.hpp"
#include "gameobject.hpp"
#include "../events/event/event.hpp"
#include "../graphics/window/window.hpp"

namespace Viper::Components {
    class Input : public Component {
    public:
        VIPER_COMPONENT_DECLARE( Input );

        Input();
        Input(GameObject* Parent);
    
        void OnEvent(Viper::Events::Event& e) override;

        // Keyboard event callbacks
        bool OnKeyboardKeyPressedEvent(Viper::Graphics::KeyboardKeyPressedEvent& Event);
        bool OnKeyboardKeyReleasedEvent(Viper::Graphics::KeyboardKeyReleasedEvent& Event);

        // Mouse event callbacks
        bool OnMouseButtonPressedEvent(Viper::Graphics::MouseButtonPressedEvent &Event);
        bool OnMouseButtonReleasedEvent(Viper::Graphics::MouseButtonReleasedEvent &Event);
    private:
        GameObject* Object;

        // Registers both pressed and held
        std::unordered_map<int, bool> KeyboardKey;

        std::unordered_map<int, bool> MouseButton; 
    };
}
