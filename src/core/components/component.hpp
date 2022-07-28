#pragma once
#include <cstddef>

namespace Viper::Components {
    enum class ComponentType : int {
        None = 0,
        Transform = 1,
        Camera = 2,
        NativeScripting = 3,
        Scripting = 4,
        SpriteRenderer = 5,

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
        
        virtual void Awake() {};
        virtual void Update( double deltaticks) {};
        virtual void Editor() {};
    };
};