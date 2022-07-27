#pragma once

#include <functional>
#include "Event.hpp"

namespace Viper {
    class EventDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& Evnt) : Evnt(Evnt) {}

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (Evnt.GetEventType() == T::GetStaticType())
            {
                Evnt.Handled |= func(static_cast<T&>(Evnt));
                return true;
            }
            return false;
        }

    private:
        Event &Evnt;
    };
}