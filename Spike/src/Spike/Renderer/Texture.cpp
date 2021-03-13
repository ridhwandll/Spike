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
    Ref<Texture2D> Texture2D::Create(Uint width, Uint height)
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

    Ref<Texture2D> Texture2D::Create(const String& path, bool flipped)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLTexture2D>::Create(path, flipped);
            case RendererAPI::API::DX11:    return Ref<DX11Texture2D>::Create(path, flipped);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Uint Texture::CalculateMipMapCount(Uint width, Uint height)
    {
        Uint levels = 1;
        while ((width | height) >> levels)
            levels++;

        return levels;
    }

    Ref<TextureCube> TextureCube::Create(const String& folderpath)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLTextureCube>::Create(folderpath);
            case RendererAPI::API::DX11:    return Ref<DX11TextureCube>::Create(folderpath);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }
}