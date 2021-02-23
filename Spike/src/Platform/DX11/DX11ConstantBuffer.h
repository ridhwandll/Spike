/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11ConstantBuffer
File Type      : h
File created on: 2021/02/15
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
        virtual uint32_t GetSize() override { return mSize; }

        virtual RendererID GetNativeBuffer() override { return (RendererID)mBuffer; }
        virtual ShaderDomain GetShaderDomain() override { return mShaderDomain; }
        virtual DataUsage GetDataUsage() override { return mDataUsage; }
    private:
        ID3D11Buffer* mBuffer;
        uint32_t mSize;
        uint32_t mBindSlot;
        void* mData;
        ShaderDomain mShaderDomain;
        DataUsage mDataUsage;
    };
}
