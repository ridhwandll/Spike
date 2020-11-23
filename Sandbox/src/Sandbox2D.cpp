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
    m_SquareVA = LightEngine::VertexArray::Create();

    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    LightEngine::Ref<LightEngine::VertexBuffer> squareVB;
    squareVB.reset(LightEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetLayout({
        { LightEngine::ShaderDataType::Float3, "a_Position" }
        });
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    LightEngine::Ref<LightEngine::IndexBuffer> squareIB;
    squareIB.reset(LightEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->SetIndexBuffer(squareIB);

    m_FlatColorShader = LightEngine::Shader::Create("assets/shaders/FlatColorShader.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(LightEngine::Timestep ts)
{
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    LightEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    LightEngine::RenderCommand::Clear();

    LightEngine::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<LightEngine::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<LightEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    LightEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    // Triangle
    //LightEngine::Renderer::Submit(m_Shader, m_VertexArray);

    LightEngine::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(LightEngine::Event& e)
{
    m_CameraController.OnEvent(e);
}
