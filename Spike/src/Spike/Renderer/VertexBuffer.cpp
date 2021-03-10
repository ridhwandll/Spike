//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "RendererAPI.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/DX11/DX11VertexBuffer.h"

namespace Spike
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferLayout layout)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(size, layout);
            case RendererAPI::API::DX11:    return Ref<DX11VertexBuffer>::Create(size, layout);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }


    Ref<VertexBuffer> VertexBuffer::Create(void* vertices, uint32_t size, VertexBufferLayout layout)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(vertices, size, layout);
            case RendererAPI::API::DX11:    return Ref<DX11VertexBuffer>::Create(vertices, size, layout);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }
}