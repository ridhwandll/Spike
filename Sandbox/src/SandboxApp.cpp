#include <LightEngine.h>

class ExampleLayer : public LightEngine::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        LE_INFO("ExampleLayer::Update");
    }

    void OnEvent(LightEngine::Event& event) override
    {
        LE_TRACE("{0}", event);
    }

};
class SandBox : public LightEngine::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new LightEngine::ImGuiLayer());
    }
    ~SandBox(){}
};

LightEngine::Application* LightEngine::CreateApplication()
{
    return new SandBox();
}
