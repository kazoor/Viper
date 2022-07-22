#pragma once
#include <utility>
#include "../../../events/event/event.hpp"
#include "../../../events/eventhandler/eventhandler.hpp"

namespace Viper::Input {

    struct MouseCursorPositionEvent : Viper::Events::Event {
        MouseCursorPositionEvent(double XPos, double YPos) : XPos(XPos), YPos(YPos) {}

        double XPos, YPos;
    };

    class MouseEvents {
    public:
        MouseEvents();

        void OnMouseCursorPositionEvent(MouseCursorPositionEvent *E);

        static std::pair<double, double> GetMousePosition();

    private:
        Events::EventBus *MouseEventHandler;
    };
}