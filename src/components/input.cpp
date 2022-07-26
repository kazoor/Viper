#include "input.hpp"
#include "../util/globals/global.hpp"
#include "../inputlayer/keyboardinputlayer.hpp"
#include "../inputlayer/mouseinputlayer.hpp"

namespace Viper::Components {
    VIPER_CLASS_DEFINE(Component, Input);

    Input::Input() {

    }

    void Input::OnAwake() {

    }

    void Input::OnUpdate(double deltatime) {
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Viper::Components::Input::OnKeyboardEventUpdate);
        Globals::GlobalsContext::EventHandler->Subscribe(this, &Viper::Components::Input::OnMouseEventUpdate);
    }

    void Input::OnEditor() {

    }

    void Input::OnKeyboardEventUpdate(Viper::Input::KeyboardInputLayerEvent *E) {
        Keys = E->Keyboard;
    }

    void Input::OnMouseEventUpdate(Viper::Input::MouseInputLayerEvent *E) {
        MouseButtons = E->Mouse;
        MousePos = E->MousePos;
        ScrollInput = E->ScrollInput;
    }

    const std::unordered_map<int, bool> &Input::GetKeys() const {
        return Keys;
    }

    const std::unordered_map<int, bool> &Input::GetMouseButtons() const {
        return MouseButtons;
    }

    const std::pair<double, double> &Input::GetMousePos() const {
        return MousePos;
    }

    const std::pair<double, double> &Input::GetScrollInput() const {
        return ScrollInput;
    }
}