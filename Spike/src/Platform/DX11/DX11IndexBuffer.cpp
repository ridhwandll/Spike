//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "DX11IndexBuffer.h"
#include "DX11Internal.h"

namespace Spike
{
    DX11IndexBuffer::DX11IndexBuffer(void* indices, Uint count)
        :m_Count(count)
    {
        D3D11_BUFFER_DESC id = {};
        id.Usage = D3D11_USAGE_DEFAULT;
        id.ByteWidth = sizeof(Uint) * count;
        id.BindFlags = D3D11_BIND_INDEX_BUFFER;
        id.CPUAccessFlags = 0;
        id.MiscFlags = 0;
        id.StructureByteStride = sizeof(Uint);

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = indices;
        sd.SysMemPitch = 0;
        sd.SysMemSlicePitch = 0;

        DX_CALL(DX11Internal::GetDevice()->CreateBuffer(&id, &sd, &m_IndexBuffer));
    }

    DX11IndexBuffer::~DX11IndexBuffer()
    {
        m_IndexBuffer->Release();
    }

    void DX11IndexBuffer::Bind() const
    {
        DX11Internal::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    }

    void DX11IndexBuffer::Unbind() const
    {
        DX11Internal::GetDeviceContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
    }
}