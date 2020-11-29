#pragma once
#include <LightEngine.h>
#include "LightEngine/Core/Base.h"


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
    LightEngine::OrthographicCameraController m_CameraController;

    // TODO: These are temporary
    LightEngine::Ref<LightEngine::Shader> m_FlatColorShader;
    LightEngine::Ref<LightEngine::VertexArray> m_SquareVA;
    LightEngine::Ref < LightEngine::Texture2D > m_CheckerboardTexture;
    glm::vec4 m_SquareColor = { 0.9f, 0.2f, 0.1f, 1.0f };
};