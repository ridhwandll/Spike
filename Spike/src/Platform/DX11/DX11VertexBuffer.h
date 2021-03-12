//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Base.h"
#include "Spike/Renderer/VertexBuffer.h"
#include <d3d11.h>

namespace Spike
{
    class DX11VertexBuffer : public VertexBuffer
    {
    public:
        DX11VertexBuffer(Uint size, VertexBufferLayout layout);
        DX11VertexBuffer(void* vertices, Uint size, VertexBufferLayout layout);
        virtual ~DX11VertexBuffer();
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void SetData(const void* data, Uint size) override;

        virtual void SetLayout(const VertexBufferLayout& layout) override { mLayout = layout; }
        virtual const VertexBufferLayout& GetLayout() const override { return mLayout; }
    private:
        VertexBufferLayout mLayout;
        ID3D11Buffer* mVertexBuffer = nullptr;
    };
}