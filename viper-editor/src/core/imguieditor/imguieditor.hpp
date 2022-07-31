#pragma once

#include <GLFW/glfw3.h>

// layer:
#include <layers/layer/layer.hpp>

// events:
#include <events/event/event.hpp>

// graphics:
#include <graphics/window/window.hpp>

#include <viper/base.hpp>
#include "fontawesome5.hpp"

// util:
#include <util/globals/global.hpp>
#include <util/input/input.hpp>
#include <util/input/keycodes.hpp>

// components:

//#include "../components/boxcollision2d.hpp"

namespace Viper {
    
    class VIPER_API ImGuiEditor : public Layers::Layer {
    public:
        ImGuiEditor(void* window);

        ~ImGuiEditor();

        void OnUpdate(Timestep::Timestep ts);

        void OnEvent(Events::Event& event);

        void Destroy();

        bool MouseScrollEvent(Events::MouseScrollEvent& E);
    private:
        void ImGui_OnInspector();

        void ImGui_OnScene();

        void ImGui_OnConsole();

        void ImGui_OnHierarchy(Timestep::Timestep ts);

        void ImGui_OnViewport( const Graphics::WindowParams_t& WindowData);

        void ImGUi_OnPlaymode();
    private:
        GLFWwindow* WindowContext;
    };
}