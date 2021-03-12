//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/IndexBuffer.h"
#include <d3d11.h>

namespace Spike
{
    class DX11IndexBuffer : public IndexBuffer
    {
    public:
        DX11IndexBuffer(void* indices, Uint count);
        virtual ~DX11IndexBuffer();

        void Bind() const override;
        void Unbind() const override;

        Uint GetCount() const override { return m_Count; }
    private:
        ID3D11Buffer* m_IndexBuffer;
        Uint m_Count;
    };
}
