//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "DX11VertexBuffer.h"
#include "DX11Internal.h"

namespace Spike
{
    /* [Spike] Dynamic Vertex Buffer [Spike] */
    DX11VertexBuffer::DX11VertexBuffer(uint32_t size, VertexBufferLayout layout)
        :mLayout(layout)
    {
        D3D11_BUFFER_DESC vbd = {};
        vbd.Usage = D3D11_USAGE_DYNAMIC;
        vbd.ByteWidth = size;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = layout.GetStride();

        DX_CALL(DX11Internal::GetDevice()->CreateBuffer(&vbd, nullptr, &mVertexBuffer)); //Create empty vertex buffer
    }

    DX11VertexBuffer::DX11VertexBuffer(void* vertices, uint32_t size, VertexBufferLayout layout)
        :mLayout(layout)
    {
        D3D11_BUFFER_DESC vbd = {};
        vbd.Usage = D3D11_USAGE_DEFAULT;
        vbd.ByteWidth = size;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = layout.GetStride();

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertices;
        sd.SysMemPitch = 0;
        sd.SysMemSlicePitch = 0;

        DX_CALL(DX11Internal::GetDevice()->CreateBuffer(&vbd, &sd, &mVertexBuffer));
    }

    DX11VertexBuffer::~DX11VertexBuffer()
    {
        mVertexBuffer->Release();
    }

    void DX11VertexBuffer::Bind() const
    {
        uint32_t stride = mLayout.GetStride();
        uint32_t offset = 0;
        DX11Internal::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    }

    void DX11VertexBuffer::Unbind() const
    {
        DX11Internal::GetDeviceContext()->IASetVertexBuffers(0, 1, nullptr, 0, 0);
    }

    void DX11VertexBuffer::SetData(const void* data, uint32_t size)
    {
        this->Bind();
        auto deviceContext = DX11Internal::GetDeviceContext();
        D3D11_MAPPED_SUBRESOURCE ms = {};
        deviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
        memcpy(ms.pData, data, size);
        deviceContext->Unmap(mVertexBuffer, 0);
        this->Bind();
    }

}