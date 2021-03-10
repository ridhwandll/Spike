//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/Pipeline.h"
#include <d3d11.h>

namespace Spike
{
    class DX11Pipeline : public Pipeline
    {
    public:
        DX11Pipeline(const PipelineSpecification& spec);
        virtual ~DX11Pipeline();
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual const PipelineSpecification& GetSpecification() const override { return m_Spec; }
        virtual void SetPrimitiveTopology(PrimitiveTopology topology) override { m_PrimitiveTopology = topology; }
    private:
        ID3D11InputLayout* m_InputLayout = nullptr;
        PipelineSpecification m_Spec;
        PrimitiveTopology m_PrimitiveTopology = PrimitiveTopology::TRIANGLELIST;
    };
}
