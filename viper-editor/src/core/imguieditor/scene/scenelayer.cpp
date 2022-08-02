#include "scenelayer.hpp"
// utils:
#include <util/globals/global.hpp>
#include <util/timer/timestep.hpp>
#include <util/physics/physics2d.hpp>

// components:
#include <components/camera.hpp>
#include <components/input.hpp>
#include <components/gameobject.hpp>
#include <components/spriterenderer.hpp>

#include <scene/sceneentity.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace Viper {
    SceneLayer::SceneLayer(void* context) : Layer("Scene"), WindowContext((GLFWwindow*)context) {
        m_Camera = new Renderer::OrthoGraphicCamera(-AspectRatio * 2.0f, AspectRatio * 2.0f, 2.0f, -2.0f, 1.0f, -1.0f);
        Renderer::Renderer2D::Instantiate();
        AspectRatio = 0.0f;

        Globals::ConsoleContext::AddLog( VIPER_ICON_SUCC " Success!", "Window has been loaded!", Globals::ConsoleSuccess );
        
        m_TexSprite = Renderer::Sprite2D::Create("resources/textures/teamspeak.jpg");
        m_TexSprite2 = Renderer::Sprite2D::Create("resources/textures/checkerboard.png");

        m_ActiveScene = CreateRef< Scene >( );

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
        delete m_Camera;
        Renderer::Renderer2D::Destroy();

        OnImGuiExit();
    };

    void SceneLayer::OnUpdate(Timestep::Timestep ts) {
        Graphics::WindowParams_t &WindowData = *(Graphics::WindowParams_t *)glfwGetWindowUserPointer(WindowContext);
        
        Renderer::RenderCommand::SetClearColor( { 0.05f, 0.05f, 0.05f, 1.0f } );
        Renderer::RenderCommand::Clear();

        Renderer::Renderer2D::BindFramebuffer();
        Renderer::RenderCommand::Clear();
        m_ActiveScene->OnUpdate( ts );
        AspectRatio = ( float )Globals::Editor::SceneW / ( float )Globals::Editor::SceneH;

        Renderer::Renderer2D::Begin(*m_Camera);
        for( int y = -20; y < 20; y++ )
            for( int x = -20; x < 20; x++ )
                Renderer::Renderer2D::DrawQuad(glm::vec2(x, y), ( x + y ) % 2 ? RendererAPI::Color(0.6f, 0.6f, 0.6f) : RendererAPI::Color(0.7f, 0.7f, 0.7f));

        static float posx = 0.0f;
        static float posy = 0.0f;
        static float rad = 0.0f;

        //Renderer::Renderer2D::DrawQuadRotated(glm::vec2(posx, posy), rad * ( 3.141592f / 180.0f ), RendererAPI::Color::Green());

        Renderer::Renderer2D::DrawTexture(glm::vec2(1.0f, 0.0f), m_TexSprite2);
        
        static float rad_ex = 1.0f;

        rad_ex += 1.0f * ts * 32.0f;

        Renderer::Renderer2D::DrawLine(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(15.0f, 15.0f, 0.0f), RendererAPI::Color::Blue());
        
        Renderer::Renderer2D::DrawQuad(glm::vec2(4.0f, 4.0f), RendererAPI::Color(0.2f, 0.2f, 1.0f));

        if(!Globals::Editor::isPlaying )
            m_Camera->SetProjection(-AspectRatio * Globals::Editor::ZoomLevel, AspectRatio * Globals::Editor::ZoomLevel, Globals::Editor::ZoomLevel, -Globals::Editor::ZoomLevel, 1.0f, -1.0f);

        Renderer::Renderer2D::End();
        Renderer::Renderer2D::UnbindFramebuffer();
        OnImGuiRender(ts);
    }

    void SceneLayer::OnEvent(Events::Event& event) {
        for(auto& go : *Globals::GlobalsContext::Gom ) {
            go->OnEvent(event);
        };
    };

    void SceneLayer::OnImGuiInit() {
        ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            ImGuiStyle &style = ImGui::GetStyle();
            ImGui::StyleColorsDark();

            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.Fonts->AddFontFromFileTTF("resources/assets/fonts/OpenSans-Bold.ttf", 16.0f);
            //io.Fonts->AddFontDefault();

            static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
            ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
            io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAS, 12.0f, &icons_config, icons_ranges );

            style.Colors[ImGuiCol_Text] = ImColor( 200, 200, 200, 255 );
            style.Colors[ImGuiCol_Button] = ImColor(35, 35, 35, 255);
            style.Colors[ImGuiCol_ButtonHovered] = ImColor(40, 40, 40, 255);
            style.Colors[ImGuiCol_ButtonActive] = ImColor(45, 45, 45, 255);

            style.Colors[ImGuiCol_FrameBg] = ImColor(14, 16, 39, 255);
            style.Colors[ImGuiCol_FrameBgHovered] = ImColor(17, 19, 47, 255);
            style.Colors[ImGuiCol_FrameBgActive] = ImColor(22, 25, 52, 255);

            style.Colors[ImGuiCol_Tab] = ImColor(17, 11, 44, 255);
            style.Colors[ImGuiCol_TabHovered] = ImColor(22, 17, 68, 255);
            style.Colors[ImGuiCol_TabActive] = ImColor(22, 17, 68, 255);
            style.Colors[ImGuiCol_TabUnfocused] = ImColor(17, 11, 44, 255);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(41, 29, 93, 255);

            style.Colors[ImGuiCol_Header] = ImColor(26, 16, 61, 255);
            style.Colors[ImGuiCol_HeaderHovered] = ImColor(37, 28, 68, 255);
            style.Colors[ImGuiCol_HeaderActive] = ImColor(36, 24, 83, 255);

            style.Colors[ImGuiCol_SliderGrab] = ImColor(50, 50, 50, 255);
            style.Colors[ImGuiCol_SliderGrabActive] = ImColor(50, 50, 50, 255);

            style.Colors[ImGuiCol_WindowBg] = ImColor(3, 3, 13, 255 );
            style.Colors[ImGuiCol_TitleBg] = ImColor(11, 9, 35, 255 );
            style.Colors[ImGuiCol_TitleBgActive] = ImColor(21, 18, 55, 255 );
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(21, 18, 55, 255 );

            style.Colors[ImGuiCol_Button] = ImColor(35, 27, 75, 255 );
            style.Colors[ImGuiCol_ButtonHovered] = ImColor(35, 27, 75, 255 );
            style.Colors[ImGuiCol_ButtonActive] = ImColor(34, 24, 81, 255 );

            style.Colors[ImGuiCol_SliderGrab] = ImColor( 50, 46, 117, 255 );
            style.Colors[ImGuiCol_SliderGrabActive] = ImColor( 65, 61, 139, 255 );

            style.Colors[ImGuiCol_ScrollbarGrab] = ImColor( 50, 46, 117, 255 );
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor( 65, 61, 139, 255 );
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor( 65, 61, 139, 255 );

            style.WindowBorderSize = 0;
            style.FrameBorderSize = 0;
            style.IndentSpacing = 0.0f;
            style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
            style.WindowPadding = ImVec2( 0, 8 );
            style.IndentSpacing = 10.0f;
            style.ScrollbarRounding = 2.0f;
            style.ScrollbarSize = 10.0f;

            ImGui_ImplGlfw_InitForOpenGL(WindowContext, true);
            ImGui_ImplOpenGL3_Init("#version 410");

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
    };

    void SceneLayer::OnImGuiEnd() {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

    void SceneLayer::OnImGuiRender(Timestep::Timestep ts) {
        OnImGuiBegin();
            m_Viewport.OnImGuiRender(ts);
            m_Filexplorer.OnImGuiRender(ts);
            m_Hierarchy.OnImGuiRender(ts);
            m_Inspector.OnImGuiRender(ts);
        OnImGuiEnd();
    };
};