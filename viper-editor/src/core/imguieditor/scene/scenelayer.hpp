#pragma once

// layers:
#include <layers/layer/layer.hpp>

// events:
#include <events/event/event.hpp>

// graphics:
#include <graphics/window/window.hpp>
#include <graphics/renderer/camera/orthographic_camera_controller.hpp>

#include <scene/scene.hpp>

#include <imguieditor/scene/editor/hierarchy.hpp>
#include <imguieditor/scene/editor/inspector.hpp>
#include <imguieditor/scene/editor/viewport.hpp>
#include <imguieditor/scene/editor/filexplorer.hpp>

namespace Viper {
    class VIPER_API SceneLayer : public Layers::Layer {
    public:
        SceneLayer(void* context);

        ~SceneLayer();

        void Destroy();

        void OnUpdate(Timestep::Timestep ts) override;

        void OnEvent(Events::Event& event) override;

        void OnImGuiInit();
        void OnImGuiExit();

        void OnImGuiBegin();
        void OnImGuiEnd();
        void OnImGuiRender(Timestep::Timestep ts);
    private:
        GLFWwindow* WindowContext;
        OrthoGraphicCameraController* m_Camera;
        Ref< Scene > m_ActiveScene;

        float m_LastFrame = 0.0f;
        float AspectRatio;


        SceneFilexplorer m_Filexplorer;
        SceneHierarchy m_Hierarchy;
        SceneInspector m_Inspector;
        SceneViewport m_Viewport;
    };
}