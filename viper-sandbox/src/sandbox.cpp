#include <viper/application.hpp>
#include <imguieditor/imguieditor.hpp>
#include <imguieditor/scene/scenelayer.hpp>
#include <layers/layer/layer.hpp>

#include <graphics/renderer/api/color.hpp>
#include <graphics/renderer/renderer.hpp>

using namespace Viper::Renderer;
using namespace Viper::RendererAPI;
using namespace Viper::Events;
using namespace Viper::Timestep;

static float m_LastFrame = 0.0f;
class SandboxLayer : public Viper::Layers::Layer {
public:
    SandboxLayer(GLFWwindow* context) : Layer("SandboxLayer"), m_PtrToWindow( context ) {
        Renderer2D::Instantiate();
        m_Width = 1280.0f;
        m_Height = 720.0f;
    };

    ~SandboxLayer() {
        Destroy();
    };

    void Destroy() {
        Renderer2D::Destroy();
    };

    void OnUpdate( Timestep ts ) {
        RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
        RenderCommand::Clear();

        float aspect = m_Width / m_Height;
        auto camera = OrthoGraphicCamera(-aspect * 2.0f, aspect * 2.0f, 2.0f, -2.0f, 1.0f, -1.0f);

        Renderer2D::Begin(camera);
        Renderer2D::DrawQuad(glm::vec2(0.0f,0.0f), Color::Blue());
        
        static float rad = 0.25f;
        rad += ts.deltatime() * 200.0f;
        Renderer2D::DrawQuadRotated(glm::vec2(1.0f,0.0f), glm::vec2(4.0f,1.0f), rad * ( 3.141592f / 180.0f ), Color(1.0f, 0.2f, 1.0f, 1.0f));
        Renderer2D::End();

        printf("ts: %.2f\n", ts.seconds());
    };

    void OnEvent(Viper::Events::Event& event) {
    };
private:
    GLFWwindow* m_PtrToWindow;
    float m_Width = 0.0f;
    float m_Height = 0.0f;
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