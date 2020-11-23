#include "lepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h" //TODO: It needs to be removed

namespace LightEngine
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColorShader;
    };

    static Renderer2DStorage* s_data;

    void Renderer2D::Init()
    {
        s_data = new Renderer2DStorage();
        s_data->QuadVertexArray = LightEngine::VertexArray::Create();

        float squareVertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" }
            });
        s_data->QuadVertexArray->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_data->QuadVertexArray->SetIndexBuffer(squareIB);

        s_data->FlatColorShader = Shader::Create("assets/shaders/FlatColorShader.glsl");
    }

    void Renderer2D::Shutdown()
    {

    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
    }

    void Renderer2D::EndScene()
    {

    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

        s_data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->QuadVertexArray);
    }

}