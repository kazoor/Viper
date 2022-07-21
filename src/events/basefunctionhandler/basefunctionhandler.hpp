#pragma once

#include "../event/event.hpp"

namespace Viper::Events {
    class HandlerFunctionBase {
    public:
        // Call the member function
        void Exec(Event *E) {
            Call(E);
        }

    private:
        // Implemented by MemberFunctionHandler
        virtual void Call(Event *E) = 0;
    };
};