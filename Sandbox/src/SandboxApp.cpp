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
    }

    void OnEvent(LightEngine::Event& event) override
    {
        if (event.GetEventType() == LightEngine::EventType::KeyPressed)
        {

        }
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
