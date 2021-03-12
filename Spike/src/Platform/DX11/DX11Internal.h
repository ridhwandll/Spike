//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserveds
#pragma once
#include "Spike/Renderer/Framebuffer.h"
#include <d3d11.h>

namespace Spike::DX11Internal
{
    void Init(HWND hwnd);
    void Shutdown();
    void Resize(Uint width, Uint height);
    void BindBackbuffer();

    void CreateDeviceAndSwapChain(HWND windowHandle);
    void CreateSampler();
    void CreateBackbuffer();
    void CreateBlendState();
    void CreateRasterizerState();
    void LogDeviceInfo();
    void BeginWireframe();
    void EndWireframe();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    IDXGISwapChain* GetSwapChain();
    ID3D11BlendState* GetBlendState();
    ID3D11SamplerState* GetCommonSampler();
    Ref<Framebuffer> GetBackbuffer();
}

namespace Spike
{
    bool CheckHResult(HRESULT hresult, std::string_view statement, std::string_view file, Uint line);

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