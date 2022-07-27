#pragma once

#include <map>
#include <utility>
#include <spdlog/spdlog.h>
#include "../util/globals/global.hpp"
#include "../util/input/inputhandler/inputhandler.hpp"
#include "../util/input/mousebuttoncodes.hpp"
#include "../layers/layer/layer.hpp"
#include "../util/input/mouse/mouseevents.hpp"
#include "../util/ref/reference.hpp"

namespace Viper::Input {
    //TODO: Fix events here
    /*struct MouseInputLayerEvent : public Viper::Events::Event {
        MouseInputLayerEvent() {}
        MouseInputLayerEvent(std::unordered_map<int, bool> Mouse, std::pair<double, double> MousePos, std::pair<double, double> ScrollInput) : Mouse(std::move(Mouse)), MousePos(std::move(MousePos)), ScrollInput(std::move(ScrollInput)) {}

        // The event sends out an unordered_map of the Mouse each update, int = key, bool = state, MousePos and Scroll input.
        std::unordered_map<int, bool> Mouse;
        std::pair<double, double> MousePos;
        std::pair<double, double> ScrollInput;
    };*/

    class MouseInputLayer : public Viper::Layers::Layer {
    public:
        MouseInputLayer() : Layer("MouseInputLayer") {}

        void OnAttach() override {

        }

        void OnDetach() override {

        }

        void OnUpdate() override {
            for(int I = MouseButton::BUTTON_1; I != MouseButton::BUTTON_MIDDLE; ++I) {
                Mouse.insert({I, Input::IsMouseKeyPressed(I)});
            }

            MousePos = Input::GetMousePosition();
            ScrollInput = Input::GetScrollInput();

            // Globals::GlobalsContext::EventHandler->Commit(g_MouseInputLayerEvent.get());

            // Reset all values after sending the event.
            Mouse.clear();
            MousePos = {0, 0};
            ScrollInput = {0, 0};
        }

        /*void OnEvent(Viper::Events::Event *Event) override {
        }*/

    private:
        std::unordered_map<int, bool> Mouse;
        std::pair<double, double> MousePos;
        std::pair<double, double> ScrollInput;
    };
}