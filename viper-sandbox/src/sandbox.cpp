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

        m_Texture = Viper::Sprite2D::Create("resources/textures/xd.webp");
        m_Texture2 = Viper::Sprite2D::Create("resources/textures/checkerboard.png");

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

        float aspect = m_Width / m_Height;
        auto camera = Viper::Renderer::OrthoGraphicCamera(-aspect * 2.0f, aspect * 2.0f, 2.0f, -2.0f, 1.0f, -1.0f);

        Viper::Renderer2D::Begin(camera);
        Viper::Renderer2D::DrawTexture({ 0.0f, 0.0f }, {2.0f, 2.0f }, m_Texture2, 8.0f, glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f));
        for( float y = 0.0f; y < 1.0f; y += 0.1f ) {
            Viper::Renderer2D::DrawTexture({ y, y }, {0.2f, 0.2f }, m_Texture, 1.0f, glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f - y));
        }
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