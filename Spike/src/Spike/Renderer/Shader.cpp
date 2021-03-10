//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "Spike/Core/Vault.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/DX11/DX11Shader.h"

namespace Spike
{
    Ref<Shader> Shader::Create(const String& filepath)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLShader>::Create(filepath);
            case RendererAPI::API::DX11:    return Ref<DX11Shader>::Create(filepath);
        }
        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }
}