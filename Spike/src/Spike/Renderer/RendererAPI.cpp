//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "RendererAPI.h"
#include "RendererAPISwitch.h"

namespace Spike
{
    #ifdef RENDERER_API_OPENGL
        RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
    #elif defined RENDERER_API_DX11
        RendererAPI::API RendererAPI::s_API = RendererAPI::API::DX11;
    #else
        #error No RendererAPI selected! RendererAPI 'NONE' is currently not supported!
    #endif
}