#pragma once
#include <cstddef>
#include "../events/event/event.hpp"
#include <util/timer/timestep.hpp>

namespace Viper::Components {
    enum class ComponentType : int {
        None = 0,
        Transform = 1,
        Camera = 2,
        NativeScripting = 3,
        Scripting = 4,
        SpriteRenderer = 5,
        Input = 6,

        BoxCollider2D = 100,
        Rigidbody2D = 101,

        TestComp
    };

    #define VIPER_COMPONENT_DECLARE( ComponentDef ) ~ComponentDef() { printf("deconstructed " #ComponentDef "\n"); }; \
        static ComponentType GetType() { return ComponentType::ComponentDef; }; \
        virtual const char* GetName() const override { return #ComponentDef; }; \
        virtual ComponentType GetComponentType() const override { return GetType(); }

    #define VIPER_COMPONENT_CLASS_TEMPLATE( ComponentDef ) class ComponentDef : public Component { \
        public: \
        VIPER_COMPONENT_DECLARE( ComponentDef ); \
        \
        ComponentDef( ) { }; \
        \
    }; \

    #define VIPER_COMPONENT_CLASS_PARENTAL_TEMPLATE( ComponentDef1, ParentDef ) class ComponentDef1 : public ParentDef { \
        public: \
        VIPER_COMPONENT_DECLARE( ComponentDef1 ); \
        \
        ComponentDef1( ) { }; \
        \
    }; \

    class Component {
    public:
        virtual ~Component() = default;
        virtual const char* GetName() const = 0;
        virtual ComponentType GetComponentType() const = 0;
        virtual bool IsComponentValid( ComponentType type ) const { return ( GetComponentType() == type ); };
        
        // Making these functions non-pure as it's not necessary to define them all in a component.
        
        // Awake: Will only init once when loaded.
        virtual void Awake() {};

        // Update: Everything related to runtime will be put inside of this function.
        virtual void Update( Timestep::Timestep ts ) {};

        // Editor: The core UI elements of a component.
        virtual void Editor() {};

        // OnEvent: Will be called when an event is fired.
        virtual void OnEvent( Viper::Events::Event& event ) {};

        bool enabled;
    };
};