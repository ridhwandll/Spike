//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/DX11/DX11IndexBuffer.h"

namespace Spike
{
    Ref<IndexBuffer> IndexBuffer::Create(void* indices, Uint count)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(indices, count);
            case RendererAPI::API::DX11:    return Ref<DX11IndexBuffer>::Create(indices, count);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }
}
