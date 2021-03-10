//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/ConstantBuffer.h"
#include <d3d11.h>

namespace Spike
{
    class DX11ConstantBuffer : public ConstantBuffer
    {
    public:
        DX11ConstantBuffer(const Ref<Shader>& shader, const String& name, void* data, const uint32_t size, const uint32_t bindSlot, ShaderDomain shaderDomain, DataUsage usage);
        ~DX11ConstantBuffer();
        virtual void Bind() override;
        virtual void* GetData() override { return mData; }
        virtual void SetData(void* data) override;
        virtual uint32_t GetSize() override { return m_Size; }

        virtual RendererID GetNativeBuffer() override { return (RendererID)m_Buffer; }
        virtual ShaderDomain GetShaderDomain() override { return m_ShaderDomain; }
        virtual DataUsage GetDataUsage() override { return mDataUsage; }
    private:
        ID3D11Buffer* m_Buffer;
        uint32_t m_Size;
        uint32_t m_BindSlot;
        void* mData;
        ShaderDomain m_ShaderDomain;
        DataUsage mDataUsage;
    };
}
