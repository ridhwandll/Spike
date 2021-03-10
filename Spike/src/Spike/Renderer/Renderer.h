//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "RenderCommand.h"
#include "EditorCamera.h"
#include "ConstantBuffer.h"
#include "Mesh.h"

namespace Spike
{
    class Mesh;
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void OnWindowResize(uint32_t width, uint32_t height);
        static void BeginScene(EditorCamera& camera);
        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void EndScene();
        static void SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform);
        static void Submit(Ref<Pipeline> pipeline, uint32_t size);

        static void UpdateStats();
        static size_t GetTotalDrawCallsCount();

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static Ref<ConstantBuffer> s_SceneCbuffer;
        static Scope<SceneData> s_SceneData;
    };
}