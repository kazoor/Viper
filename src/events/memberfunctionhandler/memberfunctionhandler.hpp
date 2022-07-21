#pragma once
#include "../event/event.hpp"
#include "../basefunctionhandler/basefunctionhandler.hpp"

namespace Viper::Events {

    template<class T, class EventType>
    class MemberFunctionHandler : public HandlerFunctionBase
    {
    public:
        typedef void (T::*MemberFunction)(EventType*);

        MemberFunctionHandler(T * Instance, MemberFunction memberFunction) : Instance{ Instance }, memberFunction{ memberFunction } {};

        void Call(Event * E) {
            // Cast event to the correct type and call member function
            (Instance->*memberFunction)(static_cast<EventType*>(E));
        }
    private:
        // Pointer to class instance
        T * Instance;

        // Pointer to member function
        MemberFunction memberFunction;
    };
}