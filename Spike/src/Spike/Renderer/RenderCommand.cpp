//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DX11/DX11RendererAPI.h"
#include "RendererAPISwitch.h"

namespace Spike
{
    #ifdef RENDERER_API_OPENGL
        Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
    #elif defined RENDERER_API_DX11
        Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<DX11RendererAPI>();
    #else
        #error No RendererAPI selected! RendererAPI 'NONE' is currently not supported!
    #endif
}