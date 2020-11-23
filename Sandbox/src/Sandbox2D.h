#pragma once
#include <LightEngine.h>


class Sandbox2D : public LightEngine::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(LightEngine::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(LightEngine::Event& e) override;
private:
    // TODO: These are temporary
    LightEngine::OrthographicCameraController m_CameraController;
    LightEngine::Ref<LightEngine::Shader> m_FlatColorShader;
    LightEngine::Ref<LightEngine::VertexArray> m_SquareVA;
    glm::vec4 m_SquareColor = { 0.9f, 0.5f, 0.8f, 1.0f };
};