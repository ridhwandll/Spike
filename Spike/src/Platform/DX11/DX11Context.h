//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/GraphicsContext.h"
#include "Spike/Renderer/Framebuffer.h"
#include <string>
#include <d3d11.h>

struct GLFWwindow;

namespace Spike
{
    class DX11Context : public GraphicsContext
    {
    public:
        DX11Context(GLFWwindow* windowHandle);
        virtual ~DX11Context();
        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}
