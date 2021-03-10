//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#include "spkpch.h"
#include "Spike/Core/Vault.h"
#include "Spike/Renderer/Renderer.h"
#include "Spike/Renderer/Renderer2D.h"
#include "Spike/Renderer/Shader.h"
#include "Platform/DX11/DX11Internal.h"

namespace Spike
{
    Ref<ConstantBuffer> Renderer::s_SceneCbuffer = nullptr;
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
    static size_t s_DrawCalls = 0;

    void Renderer::Init()
    {
        RenderCommand::Init();
        Ref<Shader> shader;

        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11: shader = Shader::Create("Spike-Editor/assets/shaders/MeshShader.hlsl"); break;
            case RendererAPI::API::OpenGL: shader = Shader::Create("Spike-Editor/assets/shaders/MeshShader.glsl"); break;
        }

        Vault::Submit<Shader>(shader);
        s_SceneCbuffer = ConstantBuffer::Create(shader, "Camera", nullptr, sizeof(SceneData), 0, ShaderDomain::VERTEX, DataUsage::DYNAMIC);
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
    }

    void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(Ref<Pipeline> pipeline, uint32_t size)
    {
        RenderCommand::DrawIndexed(pipeline, size);
    }

    void Renderer::SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform)
    {
        auto shader = mesh->GetShader();
        shader->Bind();
        mesh->m_VertexBuffer->Bind();
        mesh->m_Pipeline->Bind();
        mesh->m_IndexBuffer->Bind();

        s_SceneCbuffer->SetData(&s_SceneData->ViewProjectionMatrix);

        for (Submesh& submesh : mesh->m_Submeshes)
        {
            mesh->m_Material->Bind(submesh.MaterialIndex);
            submesh.CBuffer->SetData(&(transform * submesh.Transform));
            RenderCommand::DrawIndexedMesh(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex); s_DrawCalls++;
        }
    }

    void Renderer::UpdateStats() { s_DrawCalls = 0; }
    size_t Renderer::GetTotalDrawCallsCount() { return s_DrawCalls; }
}