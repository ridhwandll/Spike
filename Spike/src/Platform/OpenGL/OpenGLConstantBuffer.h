/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : OpenGLConstantBuffer
File Type      : h
File created on: 2021/02/21
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/ConstantBuffer.h"

namespace Spike
{
    class OpenGLConstantBuffer : public ConstantBuffer
    {
    public:
        OpenGLConstantBuffer(const Ref<Shader>& shader, const String& name, void* data, const uint32_t size, const uint32_t bindSlot, ShaderDomain shaderDomain = ShaderDomain::VERTEX, DataUsage usage = DataUsage::DYNAMIC);
        virtual ~OpenGLConstantBuffer();
        virtual void Bind() override;
        virtual void* GetData() override { return m_Data; }
        virtual DataUsage GetDataUsage() override { return m_DataUsage; }
        virtual RendererID GetNativeBuffer() override { return m_RendererID; }
        virtual ShaderDomain GetShaderDomain() override { return m_ShaderDomain; }
        virtual uint32_t GetSize() override { return m_Size; }
        virtual void SetData(void* data) override;
    private:
        String m_Name;
        void* m_Data;
        uint32_t m_BindSlot;
        uint32_t m_Size;
        ShaderDomain m_ShaderDomain;
        DataUsage m_DataUsage;
        RendererID m_RendererID;
    };
}
