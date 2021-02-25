/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Texture
File Type      : cpp
File created on: 2021/02/14
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
#include "Spike/Core/Vault.h"
#include "DX11Texture.h"
#include "DX11Internal.h"
#include <stb_image.h>

namespace Spike
{
    DX11Texture2D::DX11Texture2D(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height), m_Filepath("[Spike] Built in Texture [Spike]"), m_Name("[Spike] Built in Texture [Spike]")
    {
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.ArraySize = 1;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.Usage = D3D11_USAGE_DYNAMIC;
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.Height = m_Height;
        textureDesc.Width = m_Width;
        textureDesc.MipLevels = 1;
        textureDesc.MiscFlags = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;

        DX_CALL(DX11Internal::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_Texture2D));
        m_Loaded = true;
        CreateSRV();
    }

    DX11Texture2D::DX11Texture2D(const String& filepath)
        :m_Filepath(filepath), m_Name(Vault::GetNameWithExtension(m_Filepath))
    {
        stbi_uc* data = stbi_load(filepath.c_str(), &m_Width, &m_Height, 0, 4);
        if (data == nullptr)
            SPK_CORE_LOG_ERROR("Failed to load image from filepath '%s'!", filepath.c_str());

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA sd;
        sd.pSysMem = data;
        sd.SysMemPitch = m_Width * 4 * sizeof(unsigned char);

        DX_CALL(DX11Internal::GetDevice()->CreateTexture2D(&textureDesc, &sd, &m_Texture2D));
        m_Loaded = true;
        CreateSRV();
        free(data);
    }

    void DX11Texture2D::SetData(void* data, uint32_t size)
    {
        ID3D11DeviceContext* deviceContext = DX11Internal::GetDeviceContext();
        D3D11_MAPPED_SUBRESOURCE ms = {};
        deviceContext->Map(m_Texture2D, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, data, size);
        deviceContext->Unmap(m_Texture2D, NULL);
    }

    DX11Texture2D::~DX11Texture2D()
    {
        m_Texture2D->Release();
        m_SRV->Release();
    }

    void DX11Texture2D::CreateSRV()
    {
        //TODO: give it a proper description, dont pass a nullptr here--------------------🔽
        DX_CALL(DX11Internal::GetDevice()->CreateShaderResourceView(m_Texture2D, nullptr, &m_SRV));
    }

    void DX11Texture2D::Bind(uint32_t bindslot, ShaderDomain domain) const
    {
        auto deviceContext = DX11Internal::GetDeviceContext();
        auto sampler = DX11Internal::GetCommonSampler();

        switch (domain)
        {
            case ShaderDomain::NONE: SPK_CORE_LOG_WARN("Shader domain NONE is given, this is perfectly valid. However, the developer may not want to rely on the NONE."); break;
            case ShaderDomain::VERTEX:
            {
                deviceContext->VSSetSamplers(bindslot, 1, &sampler);
                deviceContext->VSSetShaderResources(bindslot, 1, &m_SRV);
                break;
            }
            case ShaderDomain::PIXEL:
            {
                deviceContext->PSSetSamplers(bindslot, 1, &sampler);
                deviceContext->PSSetShaderResources(bindslot, 1, &m_SRV);
                break;
            }
        }
    }


    void DX11Texture2D::ActivateSlot(uint32_t slot)
    {
    }

    void DX11Texture2D::Unbind() const
    {
    }
}