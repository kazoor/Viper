#include <glm/vec4.hpp>
#include <map>
#include <memory>
#include "transform.hpp"
#include "component.hpp"
#include "gameobject.hpp"
#include "../inputlayer/keyboardinputlayer.hpp"
#include "../inputlayer/mouseinputlayer.hpp"

namespace Viper::Components {
    class Input : public Component {
    VIPER_CLASS_DECLARATION(Input)
    public:
        Input();

        void OnAwake() override;

        void OnUpdate(double deltatime) override;

        void OnEditor() override;

        const std::unordered_map<int, bool> &GetKeys() const;

        const std::unordered_map<int, bool> &GetMouseButtons() const;

        const std::pair<double, double> &GetMousePos() const;

        const std::pair<double, double> &GetScrollInput() const;

    private:
        void OnKeyboardEventUpdate(Viper::Input::KeyboardInputLayerEvent *E);

        void OnMouseEventUpdate(Viper::Input::MouseInputLayerEvent *E);
    private:
        std::unordered_map<int, bool> Keys;

        std::unordered_map<int, bool> MouseButtons;
        std::pair<double, double> MousePos;
        std::pair<double, double> ScrollInput;
    };
}

