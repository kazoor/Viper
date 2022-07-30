#include "input.hpp"
#include "../util/globals/global.hpp"
#include "../graphics/window/window.hpp"

namespace Viper::Components {
    Input::Input() {
        this->Object = nullptr;
    }

    Input::Input(GameObject* Parent) {
        this->Object = Parent;
    }

    void Input::OnEvent(Viper::Events::Event& Event) {
        // Dispatch all the callbacks.
        Events::EventDispatcher Dispatcher(Event);
        Dispatcher.Dispatch<Viper::Graphics::KeyboardKeyPressedEvent>(VIPER_GET_EVENT_FUNC(Input::OnKeyboardKeyPressedEvent));
        Dispatcher.Dispatch<Viper::Graphics::KeyboardKeyReleasedEvent>(VIPER_GET_EVENT_FUNC(Input::OnKeyboardKeyReleasedEvent));
        Dispatcher.Dispatch<Viper::Graphics::MouseButtonPressedEvent>(VIPER_GET_EVENT_FUNC(Input::OnMouseButtonPressedEvent));
        Dispatcher.Dispatch<Viper::Graphics::MouseButtonPressedEvent>(VIPER_GET_EVENT_FUNC(Input::OnMouseButtonReleasedEvent));
    }

    // Keyboard event callbacks
    bool Input::OnKeyboardKeyPressedEvent(Viper::Graphics::KeyboardKeyPressedEvent& Event) {
        // TODO: Add check for IsHeld (now we handle both pressed and held in the same map, not too sure if we want to keep it like that).
        KeyboardKey[Event.Key] = true;
    }

    bool Input::OnKeyboardKeyReleasedEvent(Viper::Graphics::KeyboardKeyReleasedEvent& Event) {
       KeyboardKey[Event.Key] = false;
    }

    // Mouse event callbacks
    bool Input::OnMouseButtonPressedEvent(Viper::Graphics::MouseButtonPressedEvent &Event) {
        MouseButton[Event.Button] = true;
    }

    bool Input::OnMouseButtonReleasedEvent(Viper::Graphics::MouseButtonReleasedEvent &Event) {
        MouseButton[Event.Button] = true;
    }
}