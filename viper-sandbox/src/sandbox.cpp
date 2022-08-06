#include <glad/glad.h>
#include <viper/application.hpp>
#include <imguieditor/imguieditor.hpp>
#include <imguieditor/scene/scenelayer.hpp>
#include <layers/layer/layer.hpp>

#include <graphics/renderer/api/color.hpp>
#include <graphics/renderer/rendercommand.hpp>
#include <graphics/renderer/camera/orthographic_camera_controller.hpp>
#include <graphics/renderer/renderer2d.hpp>
#include <graphics/renderer/light2d.hpp>
#include <graphics/renderer/sprite2d.hpp>

#include <graphics/renderer/framebuffer.hpp>

#include <random>
#include <chrono>

using namespace Viper::Events;
using namespace Viper::Timestep;

static float m_LastFrame = 0.0f;
class SandboxLayer : public Viper::Layers::Layer {
public:
    SandboxLayer(GLFWwindow* context) : Layer("SandboxLayer"), m_PtrToWindow( context ) {
        Viper::RenderCommand::Init();
        Viper::Renderer2D::Init();

        m_Texture = Viper::Sprite2D::Create("resources/textures/checkerboard.png");
        m_Texture2 = Viper::Sprite2D::Create("resources/textures/viper.png");

        m_Lights = new Viper::Light2D( );

        m_Width = 1280.0f;
        m_Height = 720.0f;

        m_Camera = new Viper::OrthoGraphicCameraController(1280.0f/720.0f);
    };

    ~SandboxLayer() {
        Destroy();
    };

    void Destroy() {
        Viper::Renderer2D::Shutdown();
        delete m_Lights;
        delete m_Camera;
    };

    void OnUpdate( Timestep ts ) {
        Viper::RenderCommand::SetColor({ 0.05f, 0.05f, 0.05f, 1.0f });
        Viper::RenderCommand::Clear();

        printf("fps :: %.2f\n", 1.0f / ts.seconds());
        m_Camera->OnUpdate(ts);

        float aspect = m_Width / m_Height;

        Viper::Renderer2D::Begin(m_Camera->GetCamera());
        Viper::Renderer2D::DrawTexture(glm::vec2(-2.0f, -2.0f), glm::vec2(40.0f, 40.0f), m_Texture, 20.0f, glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
        Viper::Renderer2D::DrawTexture(glm::vec2(-5.5f, -3.5f), glm::vec2(2.0f, 2.0f), m_Texture2, 1.0f, glm::vec4(1.0f));
            
        Viper::Renderer2D::End();

        m_Lights->Begin( m_Camera->GetCamera() );
            m_Lights->Light(glm::vec2(0.0f, 0.0f), glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
            m_Lights->Light(glm::vec2(1.0f, 1.0f), glm::vec4(0.2f, 1.0f, 0.2f, 1.0f), 45.0f);
            m_Lights->Light(glm::vec2(-1.0f, -1.0f), glm::vec4(0.2f, 1.0f, 0.2f, 1.0f), 78.0f, glm::vec2(20.0f, 20.0f));
        m_Lights->End();
    };

    void OnEvent(Viper::Events::Event& event) {
        m_Camera->OnEvent(event);
    };
private:
    GLFWwindow* m_PtrToWindow;
    float m_Width = 0.0f;
    float m_Height = 0.0f;

    Viper::Ref< Viper::Sprite2D > m_Texture, m_Texture2;
    Viper::Light2D* m_Lights;
    Viper::OrthoGraphicCameraController* m_Camera;
};

class CTest : public Viper::Application {
public:
    CTest() {
    };

    void ApplyLayers(void* appcontext) override {
        PushLayer(new SandboxLayer(reinterpret_cast< GLFWwindow* >( appcontext )));
    };
};
int main() {
    auto editor = new CTest();
    editor->Run();
    
    delete editor;
    return 0;
};