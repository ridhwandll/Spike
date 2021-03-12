//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/ConstantBuffer.h"

namespace Spike
{
    class OpenGLConstantBuffer : public ConstantBuffer
    {
    public:
        OpenGLConstantBuffer(const Ref<Shader>& shader, const String& name, void* data, const Uint size, const Uint bindSlot, ShaderDomain shaderDomain = ShaderDomain::VERTEX, DataUsage usage = DataUsage::DYNAMIC);
        virtual ~OpenGLConstantBuffer();
        virtual void Bind() override;
        virtual void* GetData() override { return m_Data; }
        virtual DataUsage GetDataUsage() override { return m_DataUsage; }
        virtual RendererID GetNativeBuffer() override { return m_RendererID; }
        virtual ShaderDomain GetShaderDomain() override { return m_ShaderDomain; }
        virtual Uint GetSize() override { return m_Size; }
        virtual void SetData(void* data) override;
    private:
        String m_Name;
        void* m_Data;
        Uint m_BindSlot;
        Uint m_Size;
        ShaderDomain m_ShaderDomain;
        DataUsage m_DataUsage;
        RendererID m_RendererID;
    };
}
