/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : RendererAPI
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