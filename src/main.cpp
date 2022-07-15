#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <ImGui/imgui.h>
#include "window/window.hpp"

int main()
{
    Window *window = new Window(800, 600, "Demo");
    return 0;
}