#pragma once

#include <string>

#define BIT(X) (1 << X)

namespace Viper {
    enum class EventType {
        None = 0,
        WindowClose, WindowFrameBufferSize, WindowResize, WindowFocus,
        KeyPress,
        MouseButtonPress, MouseMovement, MouseScroll
    };

    enum EventCategory {
        None = 0,
        WindowEvent     = BIT(0),
        InputEvent      = BIT(1),
        KeyboardEvent   = BIT(2),
        MouseEvent      = BIT(3)
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class Event {
        friend class EventDispatcher;
    public:
        Event() {}
        virtual ~Event() = default;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory Category) {
            return GetCategoryFlags() & Category;
        }

    protected:
        bool Handled = false;
    };
}