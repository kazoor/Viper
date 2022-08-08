#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>
#include <ImGui/imgui.h>
#include "scene.hpp"
#include <graphics/renderer/framebuffer.hpp>
#include <glm/vec2.hpp>

namespace Viper {
    class SceneViewport {
    public:
        SceneViewport() = default;
        SceneViewport( Scene* SceneContext, void* WindowContext );

        void OnImGuiRender(Timestep::Timestep ts, const Ref< FrameBuffer >& framebuffer);
        void OnImGuiScene(Timestep::Timestep ts, const Ref< FrameBuffer >& framebuffer);
        void OnImGuiPlay(Timestep::Timestep ts);
        void OnImGuiShader(Timestep::Timestep ts);

        const glm::vec2& GetViewportSize() const { return ViewportSize; };
    private:
        Scene* m_Context;
        void* m_WindowContext;
        glm::vec2 ViewportSize{0.0f,0.0f};
    };
};