/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11RendererAPI
File Type      : cpp
File created on: 2021/02/23
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
#include "DX11RendererAPI.h"
#include "Spike/Core/Application.h"
#include "DX11Internal.h"
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Spike
{
    void DX11RendererAPI::Init()
    {
    }

    void DX11RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        DX11Internal::Resize(width, height);
    }

    void DX11RendererAPI::SetClearColor(const glm::vec4& color)
    {
        m_ClearColor = color;
    }

    void DX11RendererAPI::Clear()
    {
        DX11Internal::GetBackbuffer()->Clear(m_ClearColor);
    }

    void DX11RendererAPI::DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t indexCount)
    {
        uint32_t count = indexCount ? indexCount : pipeline->GetSpecification().IndexBuffer->GetCount();
        DX11Internal::GetDeviceContext()->DrawIndexed(count, 0, 0);
    }

    void DX11RendererAPI::DrawIndexedMesh(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex)
    {
        DX11Internal::GetDeviceContext()->DrawIndexed(indexCount, baseIndex, baseVertex);
    }

    void DX11RendererAPI::BindBackbuffer()
    {
        DX11Internal::BindBackbuffer();
    }

}