/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : OpenGLFramebuffer
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
#include "OpenGLFramebuffer.h"
#include "glad/glad.h"

namespace Spike
{
    static const uint32_t s_MaxFramebufferSize = 8192;

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    {

    }

    void OpenGLFramebuffer::Bind()
    {

    }

    void OpenGLFramebuffer::Unbind()
    {

    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {

    }

    void OpenGLFramebuffer::Clear(const glm::vec4& clearColor)
    {

    }

    RendererID OpenGLFramebuffer::GetColorViewID()
    {
        return (RendererID)nullptr;
    }

    RendererID OpenGLFramebuffer::GetSwapChainTarget()
    {
        return (RendererID)nullptr;
    }

    void OpenGLFramebuffer::CreateSwapChainView()
    {

    }

    void OpenGLFramebuffer::CreateColorView(FramebufferSpecification::BufferDesc desc)
    {

    }

    void OpenGLFramebuffer::Invalidate()
    {

    }

    void OpenGLFramebuffer::Clean()
    {

    }
}