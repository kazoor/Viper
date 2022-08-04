#include <viper/application.hpp>
#include <imguieditor/imguieditor.hpp>
#include <imguieditor/scene/scenelayer.hpp>
#include <layers/layer/layer.hpp>

#include <graphics/renderer/api/color.hpp>
#include <graphics/renderer/rendercommand.hpp>
#include <graphics/renderer/renderer2d.hpp>
#include <graphics/renderer/sprite2d.hpp>

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

        m_Width = 1280.0f;
        m_Height = 720.0f;
    };

    ~SandboxLayer() {
        Destroy();
    };

    void Destroy() {
        Viper::Renderer2D::Shutdown();
    };

    void OnUpdate( Timestep ts ) {
        Viper::RenderCommand::SetColor({ 0.05f, 0.05f, 0.05f, 1.0f });
        Viper::RenderCommand::Clear();

        printf("fps :: %.2f\n", 1.0f / ts.seconds());

        float aspect = m_Width / m_Height;
        auto camera = Viper::Renderer::OrthoGraphicCamera(-aspect * 5.0f, aspect * 5.0f, 5.0f, -5.0f, 1.0f, -1.0f);

        Viper::Renderer2D::Begin(camera);
        Viper::Renderer2D::DrawTexture(glm::vec2(-2.0f, -2.0f), glm::vec2(40.0f, 40.0f), m_Texture, 20.0f, glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
        Viper::Renderer2D::DrawTexture(glm::vec2(-5.5f, -3.5f), glm::vec2(2.0f, 2.0f), m_Texture2, 1.0f, glm::vec4(1.0f));
        Viper::Renderer2D::End();
    };

    void OnEvent(Viper::Events::Event& event) {
    };
private:
    GLFWwindow* m_PtrToWindow;
    float m_Width = 0.0f;
    float m_Height = 0.0f;

    Viper::Ref< Viper::Sprite2D > m_Texture, m_Texture2;
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