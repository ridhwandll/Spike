//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/DX11/DX11IndexBuffer.h"

namespace Spike
{
    Ref<IndexBuffer> IndexBuffer::Create(void* indices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(indices, size);
            case RendererAPI::API::DX11:    return Ref<DX11IndexBuffer>::Create(indices, size);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }
}
