/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Renderer
File Type      : cpp
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "Spike/Core/Vault.h"
#include "Spike/EngineAssets/EngineAssets.h"
#include "Spike/Renderer/Renderer.h"
#include "Spike/Renderer/Renderer2D.h"
#include "Spike/Renderer/Shader.h"

namespace Spike
{
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

    void Renderer::Init()
    {
        RenderCommand::Init();
#ifdef RENDERER_API_DX11
        auto shader = Shader::AddBuiltInShader(s_HLSLMeshShader, "MeshShader.hlsl");
#elif defined RENDERER_API_OPENGL
        auto shader = Shader::AddBuiltInShader(s_GLSLMeshShader, "MeshShader.glsl");
#endif
        Vault::SubmitBuiltInShader(shader);
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
        //TODO camera view pos here of s_SceneData
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(Ref<Shader> shader, Ref<Pipeline> pipeline, const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);

        pipeline->Bind();
        RenderCommand::DrawIndexed(pipeline);
    }

    void Renderer::Submit(Ref<Pipeline> pipeline, uint32_t size)
    {
        RenderCommand::DrawIndexed(pipeline, size);
    }

    void Renderer::SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform)
    {
        auto& shader = mesh->GetShader();
        shader->Bind();
        mesh->m_VertexBuffer->Bind();
        mesh->m_Pipeline->Bind();
        mesh->m_IndexBuffer->Bind();

        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

        for (Submesh& submesh : mesh->m_Submeshes)
        {
            shader->SetMat4("u_Transform", transform * submesh.Transform);
            RenderCommand::DrawIndexedMesh(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
        }
    }
}