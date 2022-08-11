#pragma once

// layers:
#include <layers/layer/layer.hpp>

// events:
#include <events/event/event.hpp>

// graphics:
#include <graphics/window/window.hpp>

#include "scene.hpp"

#include "panels/hierarchy.hpp"
#include "panels/inspector.hpp"
#include "panels/filexplorer.hpp"
#include "panels/viewport.hpp"
#include "panels/shadereditor.hpp"

#include <graphics/renderer/framebuffer.hpp>
#include <graphics/renderer/camera/orthographic_camera_controller.hpp>
#include <graphics/renderer/camera/editor_camera.hpp>

#include "sceneentity.hpp"

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
        Ref< FrameBuffer > m_FrameBuffer;

        EditorCamera m_EditorCamera;

        float m_LastFrame = 0.0f;
        float AspectRatio;

        Entity m_CameraEntity;
        SceneFilexplorer m_Filexplorer;
        SceneHierarchy m_Hierarchy;
        SceneInspector m_Inspector;
        SceneViewport m_Viewport;
        SceneShaderEditor m_ShaderEditor;
    };
}