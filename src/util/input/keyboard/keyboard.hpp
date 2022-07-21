#pragma once

#include <vector>
#include <map>
#include <functional>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "../../../events/event/event.hpp"
#include "../../../events/eventhandler/eventhandler.hpp"
#include "../../global/global.hpp"

struct KeyPressEvent : public Viper::Events::Event {
    KeyPressEvent(int Key) : KeyCode(Key) {}
    int KeyCode;
    bool IsPressed = false;
};

namespace Viper::Util::Input {

    class Keyboard {
    public:
        Keyboard() {

        };

    private:
        void OnKeyPress(KeyPressEvent *E) {

        }
    };
}