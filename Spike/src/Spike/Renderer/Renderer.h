//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "RenderCommand.h"
#include "EditorCamera.h"
#include "ConstantBuffer.h"
#include "Mesh.h"

namespace Spike::Renderer
{
    void Init();
    void Shutdown();
    void OnWindowResize(Uint width, Uint height);
    void BeginScene(EditorCamera& camera);
    void BeginScene(const Camera& camera, const glm::mat4& transform);
    void EndScene();

    void SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform);
    void Submit(Ref<Pipeline> pipeline, Uint size);

    void UpdateStats();
    size_t GetTotalDrawCallsCount();

    RendererAPI::API GetAPI();
}