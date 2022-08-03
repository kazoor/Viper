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
        Dispatcher.Dispatch<Events::KeyboardKeyPressedEvent>(VIPER_GET_EVENT_FUNC(Input::OnKeyboardKeyPressedEvent));
        Dispatcher.Dispatch<Events::KeyboardKeyReleasedEvent>(VIPER_GET_EVENT_FUNC(Input::OnKeyboardKeyReleasedEvent));
        Dispatcher.Dispatch<Events::MouseButtonPressedEvent>(VIPER_GET_EVENT_FUNC(Input::OnMouseButtonPressedEvent));
        Dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(VIPER_GET_EVENT_FUNC(Input::OnMouseButtonReleasedEvent));
    }

    // Keyboard event callbacks
    bool Input::OnKeyboardKeyPressedEvent(Events::KeyboardKeyPressedEvent& Event) {
        // TODO: Add check for IsHeld (now we handle both pressed and held in the same map, not too sure if we want to keep it like that).
        KeyboardKey[Event.Key] = true;
        return true;
    }

    bool Input::OnKeyboardKeyReleasedEvent(Events::KeyboardKeyReleasedEvent& Event) {
       KeyboardKey[Event.Key] = false;
       return true;
    }

    // Mouse event callbacks
    bool Input::OnMouseButtonPressedEvent(Events::MouseButtonPressedEvent &Event) {
        MouseButton[Event.Button] = true;
        return true;
    }

    bool Input::OnMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent &Event) {
        MouseButton[Event.Button] = true;
        return true;
    }
}