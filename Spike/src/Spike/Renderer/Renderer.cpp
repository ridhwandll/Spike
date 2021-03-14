//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#include "spkpch.h"
#include "Spike/Core/Vault.h"
#include "Spike/Renderer/Renderer.h"
#include "Spike/Renderer/Renderer2D.h"
#include "Spike/Renderer/Shader.h"
#include "Platform/DX11/DX11Internal.h"
#include "Skybox.h"

namespace Spike::Renderer
{
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    Scope<SceneData> sceneData = CreateScope<SceneData>();

    Ref<ConstantBuffer> sceneCbuffer;
    size_t drawCalls = 0;
    Ref<Skybox> skybox;

    void Init()
    {
        RenderCommand::Init();
        Ref<Shader> shader;

        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11: shader = Shader::Create("Spike-Editor/assets/shaders/HLSL/MeshShader.hlsl"); break;
            case RendererAPI::API::OpenGL: shader = Shader::Create("Spike-Editor/assets/shaders/GLSL/MeshShader.glsl"); break;
        }

        Vault::Submit<Shader>(shader);
        sceneCbuffer = ConstantBuffer::Create(shader, "Camera", nullptr, sizeof(SceneData), 0, ShaderDomain::VERTEX, DataUsage::DYNAMIC);
        skybox = Skybox::Create(TextureCube::Create("Spike-Editor/assets/skybox"));
    }

    void Shutdown()
    {
    }

    void OnWindowResize(Uint width, Uint height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    glm::mat4 projectionMatrix, viewMatrix;
    void BeginScene(EditorCamera& camera)
    {
        sceneData->ViewProjectionMatrix = camera.GetViewProjection();
        projectionMatrix = camera.GetProjection();
        viewMatrix = camera.GetViewMatrix();
    }

    void BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        sceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
    }

    void EndScene()
    {
        skybox->Render(projectionMatrix, viewMatrix);
    }

    void Submit(Ref<Pipeline> pipeline, Uint size)
    {
        RenderCommand::DrawIndexed(pipeline, size);
    }

    void SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform)
    {
        auto shader = mesh->GetShader();
        shader->Bind();
        mesh->GetVertexBuffer()->Bind();
        mesh->GetPipeline()->Bind();
        mesh->GetIndexBuffer()->Bind();

        sceneCbuffer->SetData(&sceneData->ViewProjectionMatrix);

        for (Submesh& submesh : mesh->GetSubmeshes())
        {
            mesh->GetMaterial()->Bind(submesh.MaterialIndex);
            submesh.CBuffer->SetData(&(transform * submesh.Transform));
            RenderCommand::DrawIndexedMesh(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex); drawCalls++;
        }
    }

    void UpdateStats() { drawCalls = 0; }
    Spike::RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    size_t GetTotalDrawCallsCount() { return drawCalls; }

}