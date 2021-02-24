/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Framebuffer
File Type      : cpp
File created on: 2021/02/12
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
#include "DX11Framebuffer.h"
#include "DX11Internal.h"

namespace Spike
{
    static const uint32_t sMaxFramebufferSize = 8192;
    DXGI_FORMAT SpikeFormatToDX11Format(FormatCode code)
    {
        switch (code)
        {
            case FormatCode::R32G32B32A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case FormatCode::R8G8B8A8_UNORM:     return DXGI_FORMAT_R8G8B8A8_UNORM;
            case FormatCode::D24_UNORM_S8_UINT:  return DXGI_FORMAT_D24_UNORM_S8_UINT;
        }
        SPK_CORE_LOG_ERROR("No correct DX11 format found for the given Spike Format. Returning DXGI_FORMAT_R32G32B32A32_FLOAT...");
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }

    DX11Framebuffer::DX11Framebuffer(const FramebufferSpecification& spec)
        :mSpecification(spec)
    {
        Invalidate();
    }

    void DX11Framebuffer::Invalidate()
    {
        Clean();
        mViewport.TopLeftX = 0.0f;
        mViewport.TopLeftY = 0.0f;
        mViewport.Width = static_cast<float>(mSpecification.Width);
        mViewport.Height = static_cast<float>(mSpecification.Height);
        mViewport.MinDepth = 0.0f;
        mViewport.MaxDepth = 1.0f;

        for (auto desc : mSpecification.BufferDescriptions)
        {
            if (mSpecification.SwapChainTarget)
                CreateSwapChainView();
            else
                CreateColorView(desc);
        }
    }

    void DX11Framebuffer::Clean()
    {
        mRenderTargetView.Reset();
        mSRV.Reset();
    }

    void DX11Framebuffer::Bind()
    {
        ID3D11DeviceContext* deviceContext = DX11Internal::GetDeviceContext();
        deviceContext->RSSetViewports(1, &mViewport);
        deviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), 0);
    }

    void DX11Framebuffer::Unbind() {}

    void DX11Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > sMaxFramebufferSize || height > sMaxFramebufferSize)
        {
            SPK_CORE_LOG_WARN("Attempted to resize framebuffer to %i, %i", width, height);
            return;
        }

        mSpecification.Width = width;
        mSpecification.Height = height;
        Invalidate();
    }

    void DX11Framebuffer::Clear(const glm::vec4& clearColor)
    {
        DX11Internal::GetDeviceContext()->ClearRenderTargetView(mRenderTargetView.Get(), (float*)&clearColor);
    }

    RendererID DX11Framebuffer::GetSwapChainTarget() { return (RendererID)mRenderTargetView.Get(); }
    void DX11Framebuffer::CreateSwapChainView()
    {
        ID3D11Texture2D* backBuffer;
        DX11Internal::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&backBuffer);
        DX11Internal::GetDevice()->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
        backBuffer->Release();
    }

    void DX11Framebuffer::CreateColorView(FramebufferSpecification::BufferDesc desc)
    {
        D3D11_TEXTURE2D_DESC textureDesc = {};
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};

        ID3D11Device* device = DX11Internal::GetDevice();

        textureDesc.Width = mSpecification.Width;
        textureDesc.Height = mSpecification.Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = SpikeFormatToDX11Format(desc.Format);
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = (D3D11_BIND_FLAG)desc.BindFlags;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        DX_CALL(device->CreateTexture2D(&textureDesc, nullptr, &mRenderTargetTexture));
        //https://renderdoc.org/docs/how/how_view_texture.html
        mRenderTargetTexture->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Render Target(Not a SwapChain)"), "Render Target(Not a SwapChain)");

        renderTargetViewDesc.Format = SpikeFormatToDX11Format(desc.Format);
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        DX_CALL(device->CreateRenderTargetView(mRenderTargetTexture.Get(), &renderTargetViewDesc, &mRenderTargetView));

        shaderResourceViewDesc.Format = SpikeFormatToDX11Format(desc.Format);
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;

        DX_CALL(device->CreateShaderResourceView(mRenderTargetTexture.Get(), &shaderResourceViewDesc, &mSRV));
    }
}