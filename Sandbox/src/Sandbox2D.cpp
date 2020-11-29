#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    LE_PROFILE_FUNCTION();
    m_CheckerboardTexture = LightEngine::Texture2D::Create("assets/textures/Checkerboard.png");
}
void Sandbox2D::OnDetach()
{
    LE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(LightEngine::Timestep ts)
{
    LE_PROFILE_FUNCTION();
    // Update
     m_CameraController.OnUpdate(ts);
    // Render
     LightEngine::Renderer2D::ResetStats();
    {
        LE_PROFILE_SCOPE("Renderer Preparation");
        LightEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        LightEngine::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        LE_PROFILE_SCOPE("Renderer Draw");
        LightEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
        LightEngine::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0f), { 0.9f, 0.1f, 0.3f, 1.0f });
        LightEngine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.9f, 0.1f, 0.3f, 1.0f });
        LightEngine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.8f }, { 0.2f, 0.3f, 0.9f, 1.0f });
        LightEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1 }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
        LightEngine::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);
        LightEngine::Renderer2D::EndScene();
        
        LightEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.8f };
                LightEngine::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }
        LightEngine::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    LE_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    auto stats = LightEngine::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();

}

void Sandbox2D::OnEvent(LightEngine::Event& e)
{
    m_CameraController.OnEvent(e);
}
