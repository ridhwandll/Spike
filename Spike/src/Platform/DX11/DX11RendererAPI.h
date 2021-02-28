/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11RendererAPI
File Type      : h
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
#pragma once
#include "Spike/Renderer/RendererAPI.h"
#include <glm/glm.hpp>

namespace Spike
{
    class DX11RendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t indexCount = 0) override;
        virtual void DrawIndexedMesh(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex) override;
        virtual void BindBackbuffer() override;
        virtual void BeginWireframe() override;
        virtual void EndWireframe() override;
    private:
        glm::vec4 m_ClearColor;
    };
}
