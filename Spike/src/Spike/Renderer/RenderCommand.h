//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "RendererAPI.h"

namespace Spike
{
    class RenderCommand
    {
    public:
        static void Init()
        {
            s_RendererAPI->Init();
        }

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        static void Clear()
        {
            s_RendererAPI->Clear();
        }

        static void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count = 0)
        {
            s_RendererAPI->DrawIndexed(pipeline, count);
        }

        static void DrawIndexedMesh(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex)
        {
            s_RendererAPI->DrawIndexedMesh(indexCount, baseIndex, baseVertex);
        }

        static void BindBackbuffer()
        {
            s_RendererAPI->BindBackbuffer();
        }

        static void BeginWireframe()
        {
            s_RendererAPI->BeginWireframe();
        }

        static void EndWireframe()
        {
            s_RendererAPI->EndWireframe();
        }

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
}