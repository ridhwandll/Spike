/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Shader
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
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Spike
{
    Vector<Ref<Shader>> Shader::s_AllShaders;

    Ref<Shader> Shader::Create(const String& filepath)
    {
        Ref<Shader> shader = nullptr;
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  shader = Ref<OpenGLShader>::Create(filepath);
        }
        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        s_AllShaders.push_back(shader);
        return shader;
    }

    void ShaderLibrary::Add(const String& name, const Ref<Shader>& shader)
    {
        SPK_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name = shader->GetName();
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const String& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const String& name, const String& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const String& name)
    {
        SPK_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const String& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }

}