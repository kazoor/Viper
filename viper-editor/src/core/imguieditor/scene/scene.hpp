#pragma once

// layers:
#include <layers/layer/layer.hpp>

// events:
#include <events/event/event.hpp>

// graphics:
#include <graphics/window/window.hpp>
#include <graphics/renderer/sprite2d.hpp>
#include <graphics/renderer/camera/orthographic_camera.hpp>

namespace Viper {
    class VIPER_API Scene : public Layers::Layer {
    public:
        Scene(void* context);

        ~Scene();

        void Destroy();

        void OnUpdate() override;

        void OnEvent(Events::Event& event) override;
    private:
        GLFWwindow* WindowContext;
        Renderer::OrthoGraphicCamera* m_Camera;
        Ref< Renderer::Sprite2D > m_TexSprite;
        Ref< Renderer::Sprite2D > m_TexSprite2;
        float m_LastFrame = 0.0f;
    private:
        float AspectRatio;
    };
}