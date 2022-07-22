#pragma once
#include "../transform.hpp"
#include "../../util/input/input.hpp"

namespace Viper::Components {
    class InputAPI : public Transform {
    public:
        InputAPI() { };

        bool IsKeyDown( const int key ) {
            return Input::Input::IsKeyPressed(key);
        };
    };
};