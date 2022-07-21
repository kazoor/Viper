#pragma once

#include <GLFW/glfw3.h>
#include "../../events/eventhandler/eventhandler.hpp"


namespace Viper::Global {
    Viper::Events::EventBus *Events;
    GLFWwindow *WindowContext;
}