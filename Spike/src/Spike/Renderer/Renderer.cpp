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
    struct SceneCBufferData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    struct SceneData
    {
        glm::mat4 ProjectionMatrix, ViewMatrix;
        Ref<ConstantBuffer> SceneCbuffer;
        size_t DrawCalls = 0;
        Ref<Spike::Skybox> Skybox;
        bool SkyboxActivated = true;
    };

    Scope<SceneCBufferData> sceneCBufferData = CreateScope<SceneCBufferData>();
    Scope<SceneData> sceneData = CreateScope<SceneData>();

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
        sceneData->SceneCbuffer = ConstantBuffer::Create(shader, "Camera", nullptr, sizeof(SceneCBufferData), 0, ShaderDomain::VERTEX, DataUsage::DYNAMIC);
    }

    void Shutdown()
    {
    }

    void OnWindowResize(Uint width, Uint height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void BeginScene(EditorCamera& camera)
    {
        sceneCBufferData->ViewProjectionMatrix = camera.GetViewProjection();
        sceneData->ProjectionMatrix = camera.GetProjection();
        sceneData->ViewMatrix = camera.GetViewMatrix();
    }

    void BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        sceneCBufferData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
    }

    void EndScene()
    {
        if (sceneData->Skybox && sceneData->SkyboxActivated)
            sceneData->Skybox->Render(sceneData->ProjectionMatrix, sceneData->ViewMatrix);
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

        sceneData->SceneCbuffer->SetData(&(*sceneCBufferData)); //Upload the sceneCBufferData
        for (Submesh& submesh : mesh->GetSubmeshes())
        {
            mesh->GetMaterial()->Bind(submesh.MaterialIndex);
            submesh.CBuffer->SetData(&(transform * submesh.Transform));
            RenderCommand::DrawIndexedMesh(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
            sceneData->DrawCalls++;
        }
    }

    void UpdateStats()
    {
        sceneData->DrawCalls = 0;
    }

    RendererAPI::API GetAPI()
    {
        return RendererAPI::GetAPI();
    }

    size_t GetTotalDrawCallsCount()
    {
        return sceneData->DrawCalls;
    }

    Ref<Skybox>& GetSkyboxSlot()
    {
        return sceneData->Skybox;
    }

    bool& GetSkyboxActivationBool()
    {
        return sceneData->SkyboxActivated;
    }
}