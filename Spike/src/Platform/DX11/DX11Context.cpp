/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11GraphicsContext
File Type      : cpp
File created on: 2021/02/11
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

3.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "DX11Context.h"
#include "DX11Internal.h"
#include "Spike/Core/Application.h"
#include "DX11Internal.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Spike
{
    DX11Context::DX11Context(GLFWwindow* windowHandel)
        :m_WindowHandle(windowHandel) {}

    void DX11Context::Init()
    {
        DX11Internal::Init(glfwGetWin32Window(m_WindowHandle));
    }

    void DX11Context::SwapBuffers()
    {
        if (Application::Get().GetWindow().IsVSync())
            DX11Internal::GetSwapChain()->Present(1, 0);
        else
            DX11Internal::GetSwapChain()->Present(0, 0);
    }
    DX11Context::~DX11Context() {}
}