#pragma once

#include <map>
#include <utility>
#include <spdlog/spdlog.h>
#include "../util/globals/global.hpp"
#include "../util/input/inputhandler/inputhandler.hpp"
#include "../util/input/mousebuttoncodes.hpp"
#include "../layers/layer/layer.hpp"
#include "../events/event/event.hpp"
#include "../util/input/mouse/mouseevents.hpp"
#include "../util/ref/reference.hpp"

namespace Viper::Input {
    class MouseInputLayerEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(MouseInputLayer, MouseInputLayerEvent);
        MouseInputLayerEvent(std::unordered_map<int, bool> Mouse, std::pair<double, double> MousePos, std::pair<double, double> ScrollInput) : Mouse(std::move(Mouse)), MousePos(std::move(MousePos)), ScrollInput(std::move(ScrollInput)) {}

        std::unordered_map<int, bool> Mouse;
        std::pair<double, double> MousePos;
        std::pair<double, double> ScrollInput;
    };

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

            //MAKE_REF(MouseInputLayerEvent, (Mouse, MousePos, ScrollInput));
            //Globals::GlobalsContext::EventHandler->Commit(g_MouseInputLayerEvent.get());

            // Reset all values after sending the event.
            Mouse.clear();
            MousePos = {0, 0};
            ScrollInput = {0, 0};
        }

        bool OnMouseInputLayerEvent(MouseInputLayerEvent& event) {
            // hantera mus.
            return true;
        };

        void OnEvent(Events::Event& event) override {
            Events::EventDispatcher dispatcher(event);
            dispatcher.Dispatch< MouseInputLayerEvent >(VIPER_GET_EVENT_FUNC(MouseInputLayer::OnMouseInputLayerEvent));
        }

    private:
        std::unordered_map<int, bool> Mouse;
        std::pair<double, double> MousePos;
        std::pair<double, double> ScrollInput;
    };
}