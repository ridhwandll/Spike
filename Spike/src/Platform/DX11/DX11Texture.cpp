//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

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

        DX_CALL(DX11Internal::GetDevice()->CreateShaderResourceView(m_Texture2D, nullptr, &m_SRV)); //Create the default SRV
    }

    DX11Texture2D::DX11Texture2D(const String& path, bool flipped)
        :m_Filepath(path), m_Name(Vault::GetNameWithExtension(m_Filepath))
    {
        LoadTexture(flipped);
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

    void DX11Texture2D::Bind(uint32_t bindslot, ShaderDomain domain) const
    {
        auto deviceContext = DX11Internal::GetDeviceContext();

        ID3D11SamplerState* sampler = DX11Internal::GetCommonSampler();
        deviceContext->PSSetSamplers(0, 1, &sampler);

        switch (domain)
        {
            case ShaderDomain::NONE: SPK_CORE_LOG_WARN("Shader domain NONE is given, this is perfectly valid. However, the developer may not want to rely on the NONE."); break;
            case ShaderDomain::VERTEX: deviceContext->VSSetShaderResources(bindslot, 1, &m_SRV); break;
            case ShaderDomain::PIXEL:  deviceContext->PSSetShaderResources(bindslot, 1, &m_SRV); break;
        }
    }

    void DX11Texture2D::Reload(bool flip)
    {
        LoadTexture(flip);
    }

    void DX11Texture2D::LoadTexture(bool flip)
    {
        stbi_set_flip_vertically_on_load(flip);

        stbi_uc* data = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, 0, 4);
        if (!data)
            SPK_CORE_LOG_ERROR("Failed to load image from filepath '%s'!", m_Filepath.c_str());

        ID3D11DeviceContext* deviceContext = DX11Internal::GetDeviceContext();

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 0;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //TODO: automate this format
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        DX_CALL(DX11Internal::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_Texture2D)); //Create the Enpty texture
        m_Loaded = true;

        auto rowPitch = m_Width * 4 * sizeof(unsigned char);
        deviceContext->UpdateSubresource(m_Texture2D, 0, 0, data, rowPitch, 0);

        //Create the Shader Resource View
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = -1;
        DX_CALL(DX11Internal::GetDevice()->CreateShaderResourceView(m_Texture2D, &srvDesc, &m_SRV));
        deviceContext->GenerateMips(m_SRV);

        free(data); //Always remember to free the data!
    }
}