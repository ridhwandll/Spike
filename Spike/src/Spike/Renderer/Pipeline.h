/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : VertexArray
File Type      : h
File created on: 2021/01/09
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
#include "Spike/Core/Ref.h"
#include "Spike/Renderer/VertexBuffer.h"
#include "Spike/Renderer/IndexBuffer.h"
#include "Spike/Renderer/Shader.h"

namespace Spike
{
    enum class PrimitiveTopology
    {
        UNDEFINED = 0,
        POINTLIST = 1,
        LINELIST = 2,
        LINESTRIP = 3,
        TRIANGLELIST = 4,
        TRIANGLESTRIP = 5
    };

    struct PipelineSpecification
    {
        Ref<Spike::VertexBuffer> VertexBuffer = nullptr;
        Ref<Spike::Shader> Shader = nullptr;
        Ref<Spike::IndexBuffer> IndexBuffer = nullptr;
    };

    class Pipeline : public RefCounted
    {
    public:
        virtual ~Pipeline() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const PipelineSpecification& GetSpecification() const = 0;
        virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
        static Ref<Pipeline> Create(const PipelineSpecification& spec);
    };
}