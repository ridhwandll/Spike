/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Texture
File Type      : h
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
#pragma once
#include "Spike/Renderer/Texture.h"
#include <d3d11.h>

namespace Spike
{
    class DX11Texture2D : public Texture2D
    {
    public:
        DX11Texture2D(uint32_t width, uint32_t height);
        DX11Texture2D(const String& filepath);
        ~DX11Texture2D();
        virtual void Bind(uint32_t bindslot = 0, ShaderDomain domain = ShaderDomain::VERTEX) const override;
        virtual const String GetName() const override { return m_Name; }
        virtual uint32_t GetWidth()  const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual String GetFilepath() const override { return m_Filepath; }
        virtual RendererID GetRendererID() const override { return (RendererID)m_SRV; }
        virtual void SetData(void* data, uint32_t size) override;
        virtual void ActivateSlot(uint32_t slot) override;
        virtual bool Loaded() override { return m_Loaded; };
        virtual void Unbind() const override;

        virtual bool operator ==(const Texture& other) const override { return m_SRV == ((DX11Texture2D&)other).m_SRV; }
    private:
        void CreateSRV();

    private:
        ID3D11Texture2D*          m_Texture2D;
        ID3D11ShaderResourceView* m_SRV;

        String m_Filepath;
        String m_Name;
        int m_Width;
        int m_Height;
        bool m_Loaded = false;
    };
}
