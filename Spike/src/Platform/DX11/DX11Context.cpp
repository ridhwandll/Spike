//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
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
    DX11Context::DX11Context(GLFWwindow* windowHandle)
        :m_WindowHandle(windowHandle)
    {
        SPK_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

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
    DX11Context::~DX11Context()
    {
        DX11Internal::Shutdown();
    }
}