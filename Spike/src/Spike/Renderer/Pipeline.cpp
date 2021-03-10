//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLPipeline.h"
#include "Platform/DX11/DX11Pipeline.h"

namespace Spike
{
    Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLPipeline>::Create(spec);
            case RendererAPI::API::DX11:    return Ref<DX11Pipeline>::Create(spec);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

}