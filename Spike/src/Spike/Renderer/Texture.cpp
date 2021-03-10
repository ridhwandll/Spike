//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "Texture.h"
#include "Spike/Core/Vault.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/DX11/DX11Texture.h"

namespace Spike
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLTexture2D>::Create(width, height);
            case RendererAPI::API::DX11:    return Ref<DX11Texture2D>::Create(width, height);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const String& path)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLTexture2D>::Create(path);
            case RendererAPI::API::DX11:    return Ref<DX11Texture2D>::Create(path);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    uint32_t Texture::CalculateMipMapCount(uint32_t width, uint32_t height)
    {
        uint32_t levels = 1;
        while ((width | height) >> levels)
            levels++;

        return levels;
    }

}