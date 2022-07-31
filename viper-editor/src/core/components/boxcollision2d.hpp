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

    class BoxCollider2D : public Component {
    public:
        VIPER_COMPONENT_DECLARE( BoxCollider2D );

        BoxCollider2D();

        BoxCollider2D(GameObject* Box1);

        void Update(Timestep::Timestep ts);

        void Editor() override;

        bool IsColliding(GameObject *Box2);
    private:
        Transform Tr;
        GameObject* Box1;
    };
}