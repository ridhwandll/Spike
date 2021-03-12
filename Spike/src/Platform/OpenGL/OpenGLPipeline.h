//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/Pipeline.h"

namespace Spike
{
    class OpenGLPipeline : public Pipeline
    {
    public:
        OpenGLPipeline(const PipelineSpecification& spec);
        virtual ~OpenGLPipeline();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual const PipelineSpecification& GetSpecification() const override { return m_Specification; }
        virtual void SetPrimitiveTopology(PrimitiveTopology topology) override;
    private:
        RendererID m_RendererID;
        Uint m_VertexBufferIndex = 0;
        PipelineSpecification m_Specification;
    };
}