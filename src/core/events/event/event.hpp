#pragma once
#include <functional>
namespace Viper::Events {
    #define VIPER_GET_EVENT_FUNC( ClassFunction ) [ this ]( auto&&... args ) { return this->ClassFunction( std::forward< decltype(args) >(args)...); }
    enum class EventType : int {
        None = 0,

        ApplicationStart,

        WindowResize, WindowFrameBufferSize, WindowPosition, WindowContentScale,
        WindowMaximization, WindowFocusEvent, WindowClose, WindowFocus,
        
        LayerUpdate,

        MouseCursorPosition, MouseScroll, MouseButtonPressed, MouseButtonReleased,
        KeyboardKeyPressed, KeyboardKeyReleased,
        WindowTest
    };

    #define VIPER_MAKE_EVENT( TypeDef, ClassName ) ClassName( ) { } \
        static Events::EventType GetType() { return Events::EventType::TypeDef; }; \
        virtual const char* GetName() const override { return #TypeDef; }; \
        virtual Events::EventType GetEventType() const override { return GetType(); } \
        const bool IsHandled( void ) const { return handled; }

    class Event {
    public:
        virtual ~Event() = default;
        virtual const char* GetName() const = 0; // event name
        virtual EventType GetEventType() const = 0; // event type
        virtual bool IsEventValid( EventType event ) const { return ( GetEventType() == event ); };

        bool handled = false;
    };

    class EventDispatcher {
    public:
        EventDispatcher( Event& e ) : event( e ) { };

		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (event.GetEventType() == T::GetType()) {
				event.handled |= func(static_cast<T&>(event));
				return true;
			}
			return false;
		}
    private:
        Event& event;
    };
};