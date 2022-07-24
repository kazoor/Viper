#include <glm/vec4.hpp>
#include <memory>
#include "transform.hpp"
#include "component.hpp"
#include "gameobject.hpp"
#include "../events/event/event.hpp"

namespace Viper::Components {
    struct BoxCollision2DEvent : Viper::Events::Event {
        BoxCollision2DEvent(GameObject* First, GameObject* Second) : First(First), Second(Second) {}

        GameObject* First;
        GameObject* Second;
    };

    class BoxCollision2D : public Component {
    VIPER_CLASS_DECLARATION(BoxCollision2D)
    public:
        BoxCollision2D();

        BoxCollision2D(GameObject* Box1);

        void OnAwake() override;

        void OnUpdate() override;

        void OnEditor() override;

        bool IsColliding(GameObject *Box2);
        
    private:
        bool CheckCollision(Transform T1, Transform T2);
    private:
        Transform Tr;
        GameObject* Box1;
    };
}