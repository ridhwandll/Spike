/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Framebuffer
File Type      : h
File created on: 2021/02/12
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
#include <wrl.h>

namespace Spike
{
    class DX11Framebuffer : public Framebuffer
    {
    public:
        DX11Framebuffer(const FramebufferSpecification& spec);
        virtual ~DX11Framebuffer() = default;

        virtual void Bind() override;
        virtual void Unbind() override;
        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual FramebufferSpecification& GetSpecification() override { return m_Specification; }
        virtual void Clear(const glm::vec4& clearColor) override;
        virtual RendererID GetColorViewID() override { return (RendererID)m_SRV.Get(); }
        virtual RendererID GetSwapChainTarget() override;
    private:
        void CreateSwapChainView();
        void CreateColorView(FramebufferSpecification::BufferDesc desc);
        void CreateDepthView(FramebufferSpecification::BufferDesc desc);
        bool IsDepthFormat(const FormatCode format);

        void Invalidate();
        void Clean();
    private:
        FramebufferSpecification m_Specification;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_RenderTargetTexture;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

        bool m_IsDepth = false;
        D3D11_VIEWPORT mViewport;
    };
}
