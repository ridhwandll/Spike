/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Internal
File Type      : h
File created on: 2021/02/17
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/Framebuffer.h"
#include <d3d11.h>

namespace Spike::DX11Internal
{
    void Init(HWND hwnd);
    void Shutdown();
    void Resize(uint32_t width, uint32_t height);
    void BindBackbuffer();

    void CreateDeviceAndSwapChain(HWND windowHandle);
    void CreateSampler();
    void CreateBackbuffer();
    void CreateBlendState();
    void CreateRasterizerState();
    void LogDeviceInfo();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    IDXGISwapChain* GetSwapChain();
    ID3D11BlendState* GetBlendState();
    ID3D11SamplerState* GetCommonSampler();
    Ref<Framebuffer> GetBackbuffer();
}

namespace Spike
{
    bool CheckHResult(HRESULT hresult, std::string_view statement, std::string_view file, uint32_t line);

    #if SPK_DEBUG
        #define DX_CALL(func)\
            {\
                HRESULT r = func;\
                CheckHResult((r), #func, __FILE__, __LINE__ );\
                SPK_CORE_ASSERT(SUCCEEDED(r), "DirectX11 Error!");\
            }
    #else
        #define DX_CALL(func) func
    #endif
}