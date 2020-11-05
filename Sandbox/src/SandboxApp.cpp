#include <LightEngine.h>

#include "imgui/imgui.h"
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
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("This is LightEngine!");
        ImGui::End();
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
    }
    ~SandBox(){}
};

LightEngine::Application* LightEngine::CreateApplication()
{
    return new SandBox();
}
