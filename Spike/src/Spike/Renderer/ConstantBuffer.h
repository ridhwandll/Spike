/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : ConstantBuffer
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

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/Shader.h"

namespace Spike
{
    enum class DataUsage
    {
        DEFAULT = 0,
        DYNAMIC
    };

    class ConstantBuffer : public RefCounted
    {
    public:
        virtual ~ConstantBuffer() = default;
        virtual void Bind() = 0;
        virtual void* GetData() = 0;
        virtual void SetData(void* data) = 0;
        virtual uint32_t GetSize() = 0;

        virtual RendererID GetNativeBuffer() = 0;
        virtual ShaderDomain GetShaderDomain() = 0;
        virtual DataUsage GetDataUsage() = 0;

        static Ref<ConstantBuffer> Create(const Ref<Shader>& shader, const String& name, void* data, const uint32_t size, const uint32_t bindSlot, ShaderDomain shaderDomain = ShaderDomain::VERTEX, DataUsage usage = DataUsage::DYNAMIC);
    };
}