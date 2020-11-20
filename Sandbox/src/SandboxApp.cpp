#include <LightEngine.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public LightEngine::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_cameraRotaiton(0.0f)
    {
        m_VertexArray.reset(LightEngine::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<LightEngine::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(LightEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
        LightEngine::BufferLayout layout = {
            { LightEngine::ShaderDataType::Float3, "a_Position" },
            { LightEngine::ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<LightEngine::IndexBuffer> indexBuffer;
        indexBuffer.reset(LightEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_SquareVA.reset(LightEngine::VertexArray::Create());

        float squareVertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        std::shared_ptr<LightEngine::VertexBuffer> squareVB;
        squareVB.reset(LightEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { LightEngine::ShaderDataType::Float3, "a_Position" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<LightEngine::IndexBuffer> squareIB;
        squareIB.reset(LightEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;
            in vec3 v_Position;
            in vec4 v_Color;
            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader.reset(LightEngine::Shader::Create(vertexSrc, fragmentSrc));

        std::string faltColorShaderVertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string flatColorShaderFragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;
            in vec3 v_Position;
            uniform vec3 u_Color;
            void main()
            {
                color = vec4(u_Color, 1.0f);
            }
        )";

        m_FlatColorShader.reset(LightEngine::Shader::Create(faltColorShaderVertexSrc, flatColorShaderFragmentSrc));
    }

    void OnUpdate(LightEngine::Timestep ts) override
    {
        if (LightEngine::Input::IsKeyPressed(LE_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (LightEngine::Input::IsKeyPressed(LE_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;

        if (LightEngine::Input::IsKeyPressed(LE_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        else if (LightEngine::Input::IsKeyPressed(LE_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;

        if (LightEngine::Input::IsKeyPressed(LE_KEY_A))
            m_cameraRotaiton += m_CameraRotationSpeed * ts;
 
        else if (LightEngine::Input::IsKeyPressed(LE_KEY_D))
            m_cameraRotaiton -= m_CameraRotationSpeed * ts;


        LightEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        LightEngine::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_cameraRotaiton);

        LightEngine::Renderer::BeginScene(m_Camera);

        static glm::mat4 scale = glm::scale(glm::mat4(0.1f), glm::vec3(0.1f));

        std::dynamic_pointer_cast<LightEngine::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<LightEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

        for(int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                glm::vec3 pos(x * 0.11f,y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                LightEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
            }
        }
        LightEngine::Renderer::Submit(m_Shader, m_VertexArray);

        LightEngine::Renderer::EndScene();
    }
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }
    void OnEvent(LightEngine::Event& event) override
    {
    }
private:
    std::shared_ptr<LightEngine::Shader> m_Shader;
    std::shared_ptr<LightEngine::VertexArray> m_VertexArray;

    std::shared_ptr<LightEngine::Shader> m_FlatColorShader;
    std::shared_ptr<LightEngine::VertexArray> m_SquareVA;

    LightEngine::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 5.0f;

    float m_cameraRotaiton = 0.0f;
    float m_CameraRotationSpeed = 180.0f;
    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.0f};
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
