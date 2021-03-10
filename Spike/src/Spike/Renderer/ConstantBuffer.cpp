//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "ConstantBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLConstantBuffer.h"
#include "Platform/DX11/DX11ConstantBuffer.h"

namespace Spike
{
    Ref<ConstantBuffer> ConstantBuffer::Create(const Ref<Shader>& shader, const String& name, void* data, const uint32_t size, const uint32_t bindSlot, ShaderDomain shaderDomain, DataUsage usage)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLConstantBuffer>::Create(shader, name, data, size, bindSlot, shaderDomain, usage);
            case RendererAPI::API::DX11:    return Ref<DX11ConstantBuffer>::Create(shader, name, data, size, bindSlot, shaderDomain, usage);
        }
        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }
}
