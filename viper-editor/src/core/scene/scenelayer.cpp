#include "scenelayer.hpp"

// utils:
#include <util/globals/global.hpp>
#include <util/timer/timestep.hpp>

// base:
#include <viper/base.hpp>

// scene:
#include "fontawesome5.hpp"
#include "entitycomponents.hpp"
#include "sceneentity.hpp"

// ImGui:
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/ImGuizmo.h>

// renderer:
#include <graphics/renderer/rendercommand.hpp>
#include <graphics/renderer/renderer2d.hpp>
#include <graphics/renderer/renderer3d.hpp>

namespace Viper {
    Ref< Sprite2D > m_Texture;
    SceneLayer::SceneLayer(void* context) : Layer("Scene"), WindowContext((GLFWwindow*)context) {
        //m_Camera = new OrthoGraphicCameraController(1280.0f/720.0f);

        Renderer2D::Init();
        Renderer3D::Init();

        m_Texture = Sprite2D::Create( "resources/textures/checkerboard.png" );

        m_EditorCamera = EditorCamera(90.0f, 1.0f, -1.0f, (float)1280 / (float)720.0f );

        Globals::ConsoleContext::AddLog( VIPER_ICON_SUCC " Success!", "Window has been loaded!", Globals::ConsoleSuccess );
        
        m_ActiveScene = CreateRef< Scene >( );
        FrameBuffer::FramebufferSpec_t specification;
        specification.width = 1280;
        specification.height = 720;
        specification.samplers = 1;
        m_FrameBuffer = FrameBuffer::Create( specification );

        //m_ActiveScene->CreateCameraEntity();

        /*
        // The following code is used for stress testing the 3D Renderer pipeline.
        // This will draw ~10,000 cubes onto the scene.
        for( float y = 0; y != 100.0f; y += 1.0f ) {
            for( float x = 0; x != 100.0f; x += 1.0f ) {
                auto ent = m_ActiveScene->CreateEntity();
                ent.add< SpriteRendererComponent >();
                auto& tr = ent.get< TransformComponent >( );
                tr.Translation = glm::vec3( x, y, 0.0f );
            }
        }*/

        // Create the 3d cube as a default entity.
        Entity cube = m_ActiveScene->CreateEntity("CubeMesh");
        auto& tr = cube.get< TransformComponent >();
        tr.Translation = glm::vec3(-0.5f, 0.0f, 0.0f);
        MeshComponent comp;
        comp.Type = MeshComponent::MeshType::Mesh_Cube;
        comp.color = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);
        cube.add< MeshComponent >(comp);

        Entity cube2 = m_ActiveScene->CreateEntity("LightMesh");
        auto& tr2 = cube2.get< TransformComponent >();
        tr2.Translation = glm::vec3(0.5f, 0.0f, 0.0f);
        MeshComponent comp2;
        comp2.Type = MeshComponent::MeshType::Mesh_Light;
        comp2.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        cube2.add< MeshComponent >(comp2);

        m_Viewport = SceneViewport( m_ActiveScene.get(), context );
        m_Hierarchy = SceneHierarchy( m_ActiveScene.get() );
        m_Inspector = SceneInspector( m_ActiveScene.get() );
        m_Filexplorer = SceneFilexplorer();

        OnImGuiInit();
    };

    SceneLayer::~SceneLayer() {
        Destroy();
    };

    void SceneLayer::Destroy() {
        //delete m_Camera;
        Renderer2D::Shutdown();
        Renderer3D::Shutdown();

        OnImGuiExit();
    };

    void SceneLayer::OnUpdate(Timestep::Timestep ts) {
        Graphics::WindowParams_t &WindowData = *(Graphics::WindowParams_t *)glfwGetWindowUserPointer(WindowContext);
        
        auto window_size = m_Viewport.GetViewportSize();
        if( auto spec = m_FrameBuffer->GetSpecification();
            window_size.x > 0.0f && window_size.y > 0.0f && spec.width != window_size.x || spec.height != window_size.y ) {
                m_FrameBuffer->Resize((uint32_t)window_size.x, (uint32_t)window_size.y);
                m_ActiveScene->OnViewportResize((uint32_t)window_size.x, (uint32_t)window_size.y);
                m_EditorCamera.OnViewportResize((uint32_t)window_size.x, (uint32_t)window_size.y);
            };
        
        m_EditorCamera.OnUpdate(ts);

        m_ActiveScene->OnSetEditorTransform(m_EditorCamera.GetProjection(), m_EditorCamera.GetTransform());
        
        m_FrameBuffer->Bind();
             
        RenderCommand::SetColor({ 0.05f, 0.05f, 0.05f, 1.0f });
        RenderCommand::Clear();

        m_ActiveScene->OnUpdate(ts);

        m_FrameBuffer->Unbind();

        OnImGuiRender(ts);
    }

    void SceneLayer::OnEvent(Events::Event& event) {
        m_EditorCamera.OnEvent(event);
     };

    void SceneLayer::OnImGuiInit() {
        ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            ImGuiStyle &style = ImGui::GetStyle();
            ImGui::StyleColorsDark();

            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.Fonts->AddFontFromFileTTF("resources/assets/fonts/OpenSans-Bold.ttf", 14.0f);
            //io.Fonts->AddFontDefault();

            static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
            ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
            io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAS, 12.0f, &icons_config, icons_ranges );

            style.Colors[ImGuiCol_Text] = ImVec4( 0.701f, 0.701f, 0.701f, 1.000f );
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.066f, 0.066f, 0.066f, 1.000f);
            style.Colors[ImGuiCol_Border] = ImVec4(0.303f, 0.303f, 0.303f, 0.500f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.156f, 0.156f, 0.156f, 1.000f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.171f, 0.171f, 0.171f, 1.000f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.118f, 0.118f, 0.118f, 1.000f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.137f, 0.137f, 0.137f, 1.000f);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.185f, 0.185f, 0.185f, 1.000f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.071f, 0.071f, 0.071f, 1.000f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.090f, 0.090f, 0.090f, 0.530f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.242f, 0.242f, 0.242f, 1.000f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.137f, 0.137f, 0.137f, 1.000f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.370f, 0.370f, 0.370f, 1.000f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.370f, 0.370f, 0.370f, 1.000f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.270f, 0.270f, 0.270f, 1.000f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.133f, 0.133f, 0.133f, 1.000f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.190f, 0.190f, 0.190f, 1.000f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.118f, 0.118f, 0.118f, 1.000f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.118f, 0.118f, 0.118f, 1.000f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.161f, 0.161f, 0.161f, 1.000f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.142f, 0.142f, 0.142f, 1.000f);
            style.Colors[ImGuiCol_Tab] = ImVec4(0.152f, 0.152f, 0.152f, 1.000f);
            style.Colors[ImGuiCol_TabHovered] = ImVec4(0.109f, 0.109f, 0.109f, 1.000f);
            style.Colors[ImGuiCol_TabActive] = ImVec4(0.081f, 0.081f, 0.081f, 1.000f);
            style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.190f, 0.190f, 0.190f, 1.000f);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.152f, 0.152f, 0.152f, 1.000f);
            style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.284f, 0.284f, 0.284f, 0.700f);
            style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.109f, 0.109f, 0.109f, 1.000f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.081f, 0.081f, 0.081f, 1.000f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.081f, 0.081f, 0.081f, 1.000f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.081f, 0.081f, 0.081f, 1.000f);

            style.WindowBorderSize = 0;
            style.FrameBorderSize = 0;
            style.IndentSpacing = 0.0f;
            style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
            style.WindowPadding = ImVec2( 0, 8 );
            style.IndentSpacing = 10.0f;
            style.ScrollbarRounding = 2.0f;
            style.ScrollbarSize = 10.0f;

            ImGui_ImplGlfw_InitForOpenGL(WindowContext, true);
            ImGui_ImplOpenGL3_Init("#version 460");

            style.GrabRounding = 2.0f;
    };

    void SceneLayer::OnImGuiExit() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    };

    void SceneLayer::OnImGuiBegin() {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    };

    void SceneLayer::OnImGuiEnd() {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

    void SceneLayer::OnImGuiRender(Timestep::Timestep ts) {
        OnImGuiBegin();
            m_Viewport.OnImGuiRender(ts, m_FrameBuffer);
            m_Filexplorer.OnImGuiRender(ts);
            m_Hierarchy.OnImGuiRender(ts, m_FrameBuffer);
            m_Inspector.OnImGuiRender(ts);
        OnImGuiEnd();
    };
};