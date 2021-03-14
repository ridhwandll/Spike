//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/RendererAPI.h"

namespace Spike
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewport(Uint x, Uint y, Uint width, Uint height) override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(Ref<Pipeline>& pipeline, Uint indexCount = 0) override;
        virtual void DrawIndexedMesh(Uint indexCount, Uint baseIndex, Uint baseVertex) override;
        virtual void BindBackbuffer() override;
        virtual void BeginWireframe() override;
        virtual void EndWireframe() override;
        virtual void SetDepthTest(DepthTestFunc type) override;
    };
}