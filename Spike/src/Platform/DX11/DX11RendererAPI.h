//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/RendererAPI.h"
#include <glm/glm.hpp>

namespace Spike
{
    class DX11RendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t indexCount = 0) override;
        virtual void DrawIndexedMesh(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex) override;
        virtual void BindBackbuffer() override;
        virtual void BeginWireframe() override;
        virtual void EndWireframe() override;
    private:
        glm::vec4 m_ClearColor;
    };
}
