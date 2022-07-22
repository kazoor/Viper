#pragma once
#include <utility>
#include "../../events/event/event.hpp"
#include "../../events/eventhandler/eventhandler.hpp"

namespace Viper::Input {

    struct ScrollEvent : public Viper::Events::Event {
        ScrollEvent(double X, double Y) : X(X), Y(Y) {}

        double X;
        double Y;
    };

    class Input {
    public:
        static bool IsKeyPressed(int KeyCode) { return Instance->IsKeyPressedImpl(KeyCode); }

        static bool IsMouseKeyPressed(int KeyCode) { return Instance->IsMouseKeyPressedImpl(KeyCode); }

        static std::pair<double, double> GetMousePosition() { return Instance->GetMousePositionImpl(); }

        static double GetMouseX() { return Instance->GetMouseXImpl(); }

        static double GetMouseY() { return Instance->GetMouseYImpl(); }

        static std::pair<double, double> GetScrollInput() { return Instance->GetScrollInputImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(int KeyCode) = 0;

        virtual bool IsMouseKeyPressedImpl(int KeyCode) = 0;

        virtual std::pair<double, double> GetMousePositionImpl() = 0;

        virtual double GetMouseXImpl() = 0;

        virtual double GetMouseYImpl() = 0;

        virtual std::pair<double, double> GetScrollInputImpl() = 0;
    private:
        static Input *Instance;
    };
}