#include "lepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace LightEngine
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_data;

    void Renderer2D::Init()
    {
        LE_PROFILE_FUNCTION();
        s_data = new Renderer2DStorage();
        s_data->QuadVertexArray = LightEngine::VertexArray::Create();

        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
            });
        s_data->QuadVertexArray->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_data->QuadVertexArray->SetIndexBuffer(squareIB);

        s_data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xfffffffff;
        s_data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

        s_data->TextureShader = Shader::Create("assets/shaders/FlatColorShader.glsl");
        s_data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_data->TextureShader->Bind();
        s_data->TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown()
    {
        LE_PROFILE_FUNCTION();
        delete s_data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        LE_PROFILE_FUNCTION();
        s_data->TextureShader->Bind();
        s_data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
        LE_PROFILE_FUNCTION();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        LE_PROFILE_FUNCTION();
        s_data->TextureShader->SetFloat4("u_Color", color);
        s_data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_data->WhiteTexture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_data->TextureShader->SetMat4("u_Transform", transform);

        s_data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        LE_PROFILE_FUNCTION();
        s_data->TextureShader->SetFloat4("u_Color", tintColor);
        s_data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_data->TextureShader->SetMat4("u_Transform", transform);

        s_data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->QuadVertexArray);
        texture->Unbind();
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        LE_PROFILE_FUNCTION();
        s_data->TextureShader->SetFloat4("u_Color", color);
        s_data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_data->WhiteTexture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_data->TextureShader->SetMat4("u_Transform", transform);

        s_data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        LE_PROFILE_FUNCTION();
        s_data->TextureShader->SetFloat4("u_Color", tintColor);
        s_data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });        s_data->TextureShader->SetMat4("u_Transform", transform);

        s_data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->QuadVertexArray);
        texture->Unbind();
    }

}