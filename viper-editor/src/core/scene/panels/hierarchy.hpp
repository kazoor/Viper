#pragma once
#include <util/ref/reference.hpp>
#include <util/timer/timestep.hpp>
#include <graphics/renderer/framebuffer.hpp>
namespace Viper {
    class Scene;
    class SceneHierarchy {
    public:
        SceneHierarchy() = default;
        SceneHierarchy( Scene* SceneContext );

        void OnImGuiRender( Timestep::Timestep ts, const Ref<FrameBuffer>& framebuffer );
        void OnImGuiRenderItems( Timestep::Timestep ts );
    private:
        Scene* m_Context;
        Scene* m_Editor;
    };
};