/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11VertexBuffer
File Type      : h
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
#pragma once
#include "Spike/Core/Base.h"
#include "Spike/Renderer/VertexBuffer.h"
#include <d3d11.h>

namespace Spike
{
    class DX11VertexBuffer : public VertexBuffer
    {
    public:
        DX11VertexBuffer(uint32_t size, VertexBufferLayout layout);
        DX11VertexBuffer(void* vertices, uint32_t size, VertexBufferLayout layout);
        virtual ~DX11VertexBuffer();
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void SetData(const void* data, uint32_t size) override;

        virtual void SetLayout(const VertexBufferLayout& layout) override { mLayout = layout; }
        virtual const VertexBufferLayout& GetLayout() const override { return mLayout; }
    private:
        VertexBufferLayout mLayout;
        ID3D11Buffer* mVertexBuffer = nullptr;
    };
}