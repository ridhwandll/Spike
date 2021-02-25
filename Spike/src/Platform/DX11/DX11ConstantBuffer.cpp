/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11ConstantBuffer
File Type      : cpp
File created on: 2021/02/15
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
#include "DX11ConstantBuffer.h"
#include "DX11Internal.h"

namespace Spike
{
    D3D11_USAGE SpikeUsageToDX11Usage(DataUsage usage)
    {
        switch (usage)
        {
            case DataUsage::DEFAULT: return D3D11_USAGE_DEFAULT;
            case DataUsage::DYNAMIC: return D3D11_USAGE_DYNAMIC;
        }
        return (D3D11_USAGE)-1;
    }

    DX11ConstantBuffer::DX11ConstantBuffer(const Ref<Shader>& shader, const String& name, void* data, const uint32_t size, const uint32_t bindSlot, ShaderDomain shaderDomain, DataUsage usage)
        :mBindSlot(bindSlot), mSize(size), mShaderDomain(shaderDomain), mDataUsage(usage)
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = ((size / 16) + 1) * 16; //Align by 16 bytes
        bufferDesc.Usage = SpikeUsageToDX11Usage(usage);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = (SpikeUsageToDX11Usage(usage) == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

        if (data == nullptr)
        {
            DX_CALL(DX11Internal::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &mBuffer)); //Create an empty CBuffer
        }
        else
        {
            D3D11_SUBRESOURCE_DATA sd = {};
            sd.pSysMem = data;
            sd.SysMemPitch = 0;
            sd.SysMemSlicePitch = 0;
            DX_CALL(DX11Internal::GetDevice()->CreateBuffer(&bufferDesc, &sd, &mBuffer));
        }
    }

    void DX11ConstantBuffer::Bind()
    {
        ID3D11DeviceContext* deviceContext = DX11Internal::GetDeviceContext();
        switch (mShaderDomain)
        {
            case ShaderDomain::NONE:   break;
            case ShaderDomain::VERTEX: deviceContext->VSSetConstantBuffers(mBindSlot, 1, &mBuffer); break;
            case ShaderDomain::PIXEL:  deviceContext->PSSetConstantBuffers(mBindSlot, 1, &mBuffer); break;
        }
    }

    void DX11ConstantBuffer::SetData(void* data)
    {
        auto deviceContext = DX11Internal::GetDeviceContext();
        switch (mDataUsage)
        {
            case DataUsage::DEFAULT:
                deviceContext->UpdateSubresource(mBuffer, 0, 0, data, 0, 0); this->Bind();  break;
            case DataUsage::DYNAMIC:
                D3D11_MAPPED_SUBRESOURCE ms = {};
                deviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
                memcpy(ms.pData, data, mSize);
                deviceContext->Unmap(mBuffer, 0); this->Bind(); break;
        }
    }

    DX11ConstantBuffer::~DX11ConstantBuffer()
    {
        mBuffer->Release();
    }
}

