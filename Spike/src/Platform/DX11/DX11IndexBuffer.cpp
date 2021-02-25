/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11IndexBuffer
File Type      : cpp
File created on: 2021/02/12
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
#include "DX11IndexBuffer.h"
#include "DX11Internal.h"

namespace Spike
{
    DX11IndexBuffer::DX11IndexBuffer(void* indices, uint32_t count)
        :m_Count(count)
    {
        D3D11_BUFFER_DESC id = {};
        id.Usage = D3D11_USAGE_DEFAULT;
        id.ByteWidth = sizeof(uint32_t) * count;
        id.BindFlags = D3D11_BIND_INDEX_BUFFER;
        id.CPUAccessFlags = 0;
        id.MiscFlags = 0;
        id.StructureByteStride = sizeof(uint32_t);

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