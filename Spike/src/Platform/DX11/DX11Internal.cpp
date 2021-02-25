/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Internal
File Type      : cpp
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
#include "spkpch.h"
#include "DX11Internal.h"

namespace Spike::DX11Internal
{
    ID3D11Device* g_Device = nullptr;
    ID3D11DeviceContext* g_DeviceContext = nullptr;
    IDXGISwapChain* g_SwapChain = nullptr;
    ID3D11BlendState* g_BlendState = nullptr;
    ID3D11RasterizerState* g_NormalRasterizerState = nullptr;
    ID3D11SamplerState* g_SamplerState = nullptr;
    Ref<Framebuffer>     g_Backbuffer = nullptr;
    uint32_t g_Height;
    uint32_t g_Width;

    void Init(HWND hwnd)
    {
        CreateDeviceAndSwapChain(hwnd);
        CreateRasterizerState();
        CreateBlendState();
        CreateBackbuffer();
        CreateSampler();
        LogDeviceInfo();
    }

    void Shutdown()
    {
        g_SamplerState->Release();
        g_BlendState->Release();
        g_SwapChain->Release();
        g_DeviceContext->Release();
        g_Device->Release();
    }

    void CreateSampler()
    {
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        DX_CALL(g_Device->CreateSamplerState(&samplerDesc, &g_SamplerState));
    }

    void CreateDeviceAndSwapChain(HWND windowHandle)
    {
        RECT clientRect;
        GetClientRect(windowHandle, &clientRect);
        g_Width = clientRect.right - clientRect.left;
        g_Height = clientRect.bottom - clientRect.top;
        SPK_CORE_ASSERT(windowHandle, "Window handle is null!");

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = g_Width;
        sd.BufferDesc.Height = g_Height;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.Flags = 0;// DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = windowHandle;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = 1;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        D3D_FEATURE_LEVEL featureLevels = { D3D_FEATURE_LEVEL_11_0 };
        UINT createDeviceFlags = 0;

#ifdef SPK_DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        DX_CALL(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, &sd, &g_SwapChain, &g_Device, nullptr, &g_DeviceContext));
    }

    void CreateBackbuffer()
    {
        FramebufferSpecification backbufferSpec;
        backbufferSpec.SwapChainTarget = true;
        backbufferSpec.Width = g_Width;
        backbufferSpec.Height = g_Height;
        backbufferSpec.BufferDescriptions.emplace_back(FramebufferSpecification::BufferDesc());
        g_Backbuffer = Framebuffer::Create(backbufferSpec);
    }

    void CreateBlendState()
    {
//https://takinginitiative.wordpress.com/2010/04/09/directx-10-tutorial-6-transparency-and-alpha-blending/
        D3D11_BLEND_DESC desc = {};
        desc.AlphaToCoverageEnable = false;
        desc.RenderTarget[0].BlendEnable = true;

        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        g_Device->CreateBlendState(&desc, &g_BlendState);
        g_DeviceContext->OMSetBlendState(g_BlendState, nullptr, 0xffffffff);
    }

    void BindBackbuffer() { g_Backbuffer->Bind(); }

    void Resize(uint32_t width, uint32_t height)
    {
        g_Backbuffer.Reset(); //Terminate the backbuffer
        DX_CALL(g_SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0)); //Resize the swapchain
        CreateBackbuffer();  //Create the backbuffer
        g_Backbuffer->Resize(width, height);
    }

    ID3D11Device* GetDevice()               { return g_Device;        }
    ID3D11DeviceContext* GetDeviceContext() { return g_DeviceContext; }
    IDXGISwapChain* GetSwapChain()          { return g_SwapChain;     }
    ID3D11BlendState* GetBlendState()       { return g_BlendState;    }
    ID3D11SamplerState* GetCommonSampler()  { return g_SamplerState;  }
    Ref<Framebuffer> GetBackbuffer()        { return g_Backbuffer;    }

    void LogDeviceInfo()
    {
        IDXGIFactory* factory = nullptr;
        IDXGIAdapter* adapter = nullptr;
        DXGI_ADAPTER_DESC adapterDesc;

        CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

        factory->EnumAdapters(0, &adapter);
        adapter->GetDesc(&adapterDesc);

        char videoCardDescription[128];
        std::string vendor, major, minor, release, build;
        LARGE_INTEGER driverVersion;
        wcstombs_s(NULL, videoCardDescription, 128, adapterDesc.Description, 128);

        if (adapterDesc.VendorId == 0x1002) vendor = "AMD";
        else if (adapterDesc.VendorId == 0x10DE) vendor = "NVIDIA Corporation";
        else if (adapterDesc.VendorId == 0x8086) vendor = "Intel";
        else if (adapterDesc.VendorId == 0x1414) vendor = "Microsoft";
        else                                     vendor = "Unknown vendor!";

        adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

        major = std::to_string(HIWORD(driverVersion.HighPart));
        minor = std::to_string(LOWORD(driverVersion.HighPart));
        release = std::to_string(HIWORD(driverVersion.LowPart));
        build = std::to_string(LOWORD(driverVersion.LowPart));

        SPK_CORE_LOG_INFO("DirectX Info:");
        SPK_CORE_LOG_INFO(" Vendor: %s", vendor.c_str());
        SPK_CORE_LOG_INFO(" Renderer: %s", videoCardDescription);
        SPK_CORE_LOG_INFO(" Version: %s.%s.%s.%s", major.c_str(), minor.c_str(), release.c_str(), build.c_str());
    }

    void CreateRasterizerState()
    {
        D3D11_RASTERIZER_DESC rasterDesc{};

        rasterDesc.CullMode = D3D11_CULL_NONE;
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.DepthClipEnable = true;

        DX_CALL(g_Device->CreateRasterizerState(&rasterDesc, &g_NormalRasterizerState));
        g_DeviceContext->RSSetState(g_NormalRasterizerState);
    }
}

namespace Spike
{
    bool CheckHResult(HRESULT hresult, std::string_view statement, std::string_view file, uint32_t line)
    {
        if (FAILED(hresult))
        {
            LPSTR buffer;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hresult, 0, reinterpret_cast<LPSTR>(&buffer), 0, nullptr);
            SPK_CORE_LOG_ERROR("[%s(%d)] {%s} failed with error: %s", file.data(), line, statement.data(), buffer);
            return false;
        }
        return true;
    }
}