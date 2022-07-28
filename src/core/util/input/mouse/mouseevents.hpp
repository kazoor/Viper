#pragma once
#include <utility>
#include "../../../events/event/event.hpp"

namespace Viper {
    class MouseCursorPositionEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(MouseCursorPosition, MouseCursorPositionEvent);
        MouseCursorPositionEvent( double xpos, double ypos ) : x( xpos ), y( ypos ) { };
        double x, y;
    };
    //struct MouseCursorPositionEvent : Viper::Events::Event {
    //    MouseCursorPositionEvent(double XPos, double YPos) : XPos(XPos), YPos(YPos) {}
//
    //    double XPos, YPos;
    //};
//
    //class MouseEvents {
    //public:
    //    MouseEvents();
//
    //    void OnMouseCursorPositionEvent(MouseCursorPositionEvent *E);
//
    //    static std::pair<double, double> GetMousePosition();
    //};
}