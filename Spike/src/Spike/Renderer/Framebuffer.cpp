//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "Framebuffer.h"
#include "Spike/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/DX11/DX11Framebuffer.h"

namespace Spike
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLFramebuffer>::Create(spec);
            case RendererAPI::API::DX11:    return Ref<DX11Framebuffer>::Create(spec);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

}