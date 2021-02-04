/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Texture
File Type      : cpp
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "Texture.h"
#include "Spike/Core/Vault.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Spike
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        Ref<Texture2D> texture;
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  texture = Ref<OpenGLTexture2D>::Create(width, height);
        }
        return Vault::SubmitTexture2D(texture);
    }

    Ref<Texture2D> Texture2D::Create(const String& path)
    {
        Ref<Texture2D> texture;
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  texture = Ref<OpenGLTexture2D>::Create(path);
        }
        return Vault::SubmitTexture2D(texture);
    }

    Ref<Texture2D> Texture2D::Create(const String& path, bool flipVertically, bool srgb)
    {
        Ref<Texture2D> texture;
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  texture = Ref<OpenGLTexture2D>::Create(path, flipVertically, srgb);
        }
        Vault::SubmitTexture2D(texture);
        return texture;
    }

    uint32_t Texture::CalculateMipMapCount(uint32_t width, uint32_t height)
    {
        uint32_t levels = 1;
        while ((width | height) >> levels)
            levels++;

        return levels;
    }

}