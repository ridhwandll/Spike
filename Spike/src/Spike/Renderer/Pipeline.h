//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
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
        virtual void BindSpecificationObjects() const = 0;
        virtual void Unbind() const = 0;

        virtual PipelineSpecification& GetSpecification() = 0;
        virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
        static Ref<Pipeline> Create(const PipelineSpecification& spec);
    };
}