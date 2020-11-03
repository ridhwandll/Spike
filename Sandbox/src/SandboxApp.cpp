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
        if (LightEngine::Input::IsKeyPressed(LE_KEY_TAB))
            LE_TRACE("Tab key is pressed (poll)!");
    }

    void OnEvent(LightEngine::Event& event) override
    {
        if (event.GetEventType() == LightEngine::EventType::KeyPressed)
        {
            LightEngine::KeyPressedEvent& e = (LightEngine::KeyPressedEvent&)event;
            if (e.GetKeyCode() == LE_KEY_TAB)
                LE_TRACE("Tab key is pressed (event)!");
            LE_TRACE("{0}", (char)e.GetKeyCode());
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
