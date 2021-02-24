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
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "DX11Internal.h"

namespace Spike::DX11Internal
{
    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mDeviceContext = nullptr;
    IDXGISwapChain* mSwapChain = nullptr;
    ID3D11BlendState* mBlendState = nullptr;
    ID3D11SamplerState* mSamplerState = nullptr;
    Ref<Framebuffer>     mBackbuffer = nullptr;
    uint32_t mHeight;
    uint32_t mWidth;

    void Init(HWND hwnd)
    {
        CreateDeviceAndSwapChain(hwnd);
        CreateBlendState();
        CreateBackbuffer();
        CreateSampler();
        LogDeviceInfo();
    }

    void Shutdown()
    {
        mSamplerState->Release();
        mBlendState->Release();
        mSwapChain->Release();
        mDeviceContext->Release();
        mDevice->Release();
    }

    void CreateSampler()
    {
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        DX_CALL(mDevice->CreateSamplerState(&samplerDesc, &mSamplerState));
    }

    void CreateDeviceAndSwapChain(HWND windowHandle)
    {
        RECT clientRect;
        GetClientRect(windowHandle, &clientRect);
        mWidth = clientRect.right - clientRect.left;
        mHeight = clientRect.bottom - clientRect.top;
        SPK_CORE_ASSERT(windowHandle, "Window handle is null!");

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = mWidth;
        sd.BufferDesc.Height = mHeight;
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
        DX_CALL(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, &sd, &mSwapChain, &mDevice, nullptr, &mDeviceContext));
    }

    void CreateBackbuffer()
    {
        FramebufferSpecification backbufferSpec;
        backbufferSpec.SwapChainTarget = true;
        backbufferSpec.Width = mWidth;
        backbufferSpec.Height = mHeight;
        backbufferSpec.BufferDescriptions.emplace_back(FramebufferSpecification::BufferDesc());
        mBackbuffer = Framebuffer::Create(backbufferSpec);
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
        mDevice->CreateBlendState(&desc, &mBlendState);
        mDeviceContext->OMSetBlendState(mBlendState, nullptr, 0xffffffff);
    }

    void BindBackbuffer() { mBackbuffer->Bind(); }

    void Resize(uint32_t width, uint32_t height)
    {
        mBackbuffer.Reset(); //Terminate the backbuffer
        DX_CALL(mSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0)); //Resize the swapchain
        CreateBackbuffer();  //Create the backbuffer
        mBackbuffer->Resize(width, height);
    }

    ID3D11Device* GetDevice()               { return mDevice;        }
    ID3D11DeviceContext* GetDeviceContext() { return mDeviceContext; }
    IDXGISwapChain* GetSwapChain()          { return mSwapChain;     }
    ID3D11BlendState* GetBlendState()       { return mBlendState;    }
    ID3D11SamplerState* GetCommonSampler()  { return mSamplerState;  }
    Ref<Framebuffer> GetBackbuffer()        { return mBackbuffer;    }

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