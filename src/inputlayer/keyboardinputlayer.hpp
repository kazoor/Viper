#pragma once

#include <map>
#include <utility>
#include "../util/input/inputhandler/inputhandler.hpp"
#include "../util/input/keycodes.hpp"
#include "../layers/layer/layer.hpp"
#include "../events/event/event.hpp"

namespace Viper::Input {
    struct KeyboardInputLayerEvent : public Viper::Events::Event {
        KeyboardInputLayerEvent(std::unordered_map<int, bool> Keyboard) : Keyboard(std::move(Keyboard)) {}

        // The event sends out an unordered_map of the keyboard each update, int = key, bool = state
        std::unordered_map<int, bool> Keyboard;
    };

    class KeyboardInputLayer : public Viper::Layers::Layer {
    public:
        KeyboardInputLayer() : Layer("KeyboardInputLayer") {}

        void OnAttach() override {

        }

        void OnDetach() override {

        }

        void OnUpdate() override {
            for(int I = Key::SPACE; I != Key::MENU; ++I) {
                Keyboard.insert({I, Input::IsKeyPressed(I)});
            }
        }

        void OnEvent(Viper::Events::Event *Event) override {
            Globals::GlobalsContext::EventHandler->Commit(new KeyboardInputLayerEvent(Keyboard));
            Keyboard.clear(); // Reset the keyboard map when the event has been sent out.
        }

    private:
        std::unordered_map<int, bool> Keyboard;
    };
}