#include "transform.hpp"
#include "component.hpp"
#include "gameobject.hpp"
#include <glm/vec4.hpp>
#include <memory>

namespace Viper::Components {
    class SpriteRenderer : public Component {
        VIPER_CLASS_DECLARATION( SpriteRenderer )
    public:
        SpriteRenderer();
        
        SpriteRenderer(GameObject* obj);

        SpriteRenderer(GameObject* obj, glm::vec4 col );

        void OnAwake() override;
        
        void OnUpdate(double deltatime) override;

        void OnEditor() override;

        glm::vec4 color;
    private:
        Transform tr;
        GameObject* object;
    };
}