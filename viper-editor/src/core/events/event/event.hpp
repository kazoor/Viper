#pragma once
#include <functional>
#include <viper/viper.hpp>
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

    class VIPER_API Event {
    public:
        virtual ~Event() = default;
        virtual const char* GetName() const = 0; // event name
        virtual EventType GetEventType() const = 0; // event type
        virtual bool IsEventValid( EventType event ) const { return ( GetEventType() == event ); };

        bool handled = false;
    };

    class VIPER_API EventDispatcher {
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

    class VIPER_API MouseScrollEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(MouseScroll, MouseScrollEvent);
        MouseScrollEvent( double xpos, double ypos ) : x( xpos ), y( ypos ) { };
        double x, y;
    };

    class VIPER_API WindowFrameBufferSizeEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowFrameBufferSize, WindowFrameBufferSizeEvent);
        WindowFrameBufferSizeEvent(int Width, int Height) : Width(Width), Height(Height) { }
        int Width, Height;
    };

    class VIPER_API WindowResizeEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowResize, WindowResizeEvent);
        WindowResizeEvent(int Width, int Height) : Height(Height), Width(Width) { }

        int Width, Height;
    };

    class VIPER_API WindowPositionEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowPosition, WindowPositionEvent);
        WindowPositionEvent(int X, int Y) : X(X), Y(Y) { }

        int X, Y;
    };

    class VIPER_API WindowContentScaleEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowContentScale, WindowContentScaleEvent);
        WindowContentScaleEvent(float XScale, float YScale) : XScale(XScale), YScale(YScale) { }

        float XScale, YScale;
    };

    class VIPER_API WindowMaximizationEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowMaximization, WindowMaximizationEvent);
        WindowMaximizationEvent(int Maximized) : Maximized(Maximized) { }

        int Maximized;
    };

    class VIPER_API WindowFocusEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowFocus, WindowFocusEvent);
        WindowFocusEvent(int Focused) : Focused(Focused) { }

        int Focused;
    };

    class VIPER_API WindowCloseEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(WindowClose, WindowCloseEvent);
    };

    class VIPER_API MouseCursorPositionEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(MouseCursorPosition, MouseCursorPositionEvent);
        MouseCursorPositionEvent( double xpos, double ypos ) : x( xpos ), y( ypos ) { };
        double x, y;
    };

    class VIPER_API KeyboardKeyPressedEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(KeyboardKeyPressed, KeyboardKeyPressedEvent);
        KeyboardKeyPressedEvent(int Key, bool IsHeld) : Key(Key), IsHeld(IsHeld) {}

        int Key;
        bool IsHeld; // Key is held down.
    };

    class VIPER_API KeyboardKeyReleasedEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(KeyboardKeyReleased, KeyboardKeyReleasedEvent);
        KeyboardKeyReleasedEvent(int Key) : Key(Key) {}

        int Key;
    };

    class VIPER_API MouseButtonPressedEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(MouseButtonPressed, MouseButtonPressedEvent);
        MouseButtonPressedEvent(int Button) : Button(Button) {}

        int Button;
    };

    class VIPER_API MouseButtonReleasedEvent : public Events::Event {
    public:
        VIPER_MAKE_EVENT(MouseButtonReleased, MouseButtonReleasedEvent);
        MouseButtonReleasedEvent(int Button) : Button(Button) {}

        int Button;
    };
};