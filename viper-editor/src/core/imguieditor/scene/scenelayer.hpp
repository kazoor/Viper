#pragma once

// layers:
#include <layers/layer/layer.hpp>

// events:
#include <events/event/event.hpp>

// graphics:
#include <graphics/window/window.hpp>
#include <graphics/renderer/sprite2d.hpp>
#include <graphics/renderer/camera/orthographic_camera.hpp>

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
        Renderer::OrthoGraphicCamera* m_Camera;
        Ref< Renderer::Sprite2D > m_TexSprite;
        Ref< Renderer::Sprite2D > m_TexSprite2;
        Ref< Scene > m_ActiveScene;

        float m_LastFrame = 0.0f;
        float AspectRatio;


        Editor::SceneFilexplorer m_Filexplorer;
        Editor::SceneHierarchy m_Hierarchy;
        Editor::SceneInspector m_Inspector;
        Editor::SceneViewport m_Viewport;
    };
}